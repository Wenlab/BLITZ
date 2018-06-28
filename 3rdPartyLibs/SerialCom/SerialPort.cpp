//////////////////////////////////////////////////////////////////////////    
/// COPYRIGHT NOTICE    
/// Copyright (c) 2009, Huazhong University of Science and Technology(华中科技大学) tickTick Group  
/// All rights reserved.    
///     
/// @file    SerialPort.cpp      
/// @brief   For serial communication    
///    
///    
///    
/// 
/// Apr. 28, 2018
/// @version 1.0       
/// @author  Jun Lu (卢俊)      
/// @E-mail：lujun.hust@gmail.com    
/// @date    2010/03/19    
///     
///    
///    Modified by Wenbin Yang (bysin7@gmail.com) 
//////////////////////////////////////////////////////////////////////////    
    
#include "SerialPort.h"    
#include <process.h>    
#include <iostream>    
  
/** the flag of thread exit */  
bool CSerialPort::s_bExit = false;  
/** When no data presented, sleep until next inquiry (unit: seconds) */  
const UINT SLEEP_TIME_INTERVAL = 5;  
  
CSerialPort::CSerialPort(void)  
    : m_hListenThread(INVALID_HANDLE_VALUE)  
{  
    m_hComm = INVALID_HANDLE_VALUE;  
    m_hListenThread = INVALID_HANDLE_VALUE;  
    InitializeCriticalSection(&m_csCommunicationSync);  
  
}  
  
CSerialPort::~CSerialPort(void)  
{  
    CloseListenTread();  
    ClosePort();  
    DeleteCriticalSection(&m_csCommunicationSync);  
}  
  
bool CSerialPort::InitPort(UINT portNo /*= 1*/, UINT baud /*= CBR_9600*/, char parity /*= 'N'*/,  
    UINT databits /*= 8*/, UINT stopsbits /*= 1*/, DWORD dwCommEvents /*= EV_RXCHAR*/)  
{  
  
    /** temporal var, to convert arguments to string for constructing DCB structure */  
    char szDCBparam[50];  
    sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);  
  
    /** Open the oprt, DO NOT add protection since there is one in the function body */  
    if (!openPort(portNo))  
    {  
        return false;  
    }  
   
    EnterCriticalSection(&m_csCommunicationSync);  
  
    /** Any error occur? */  
    BOOL bIsSuccess = TRUE;  
  
    /** user can define the size of buffer to overwrite default value,
	* make sure the buffer is big enough to avoid leaking
    */  
    /*if (bIsSuccess ) 
    { 
    bIsSuccess = SetupComm(m_hComm,10,10); 
    }*/  
  
    /** Set all timeouts to 0, means no timeout */  
    COMMTIMEOUTS  CommTimeouts;  
    CommTimeouts.ReadIntervalTimeout = 0;  
    CommTimeouts.ReadTotalTimeoutMultiplier = 0;  
    CommTimeouts.ReadTotalTimeoutConstant = 0;  
    CommTimeouts.WriteTotalTimeoutMultiplier = 0;  
    CommTimeouts.WriteTotalTimeoutConstant = 0;  
    if (bIsSuccess)  
    {  
        bIsSuccess = SetCommTimeouts(m_hComm, &CommTimeouts);  
    }  
  
    DCB  dcb;  
    if (bIsSuccess)  
    {  
        // convert ansic to unicode    
        DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);  
        wchar_t *pwText = new wchar_t[dwNum];  
        if (!MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum))  
        {  
            bIsSuccess = TRUE;  
        }  
  
        /** get the com state and build com DCB */  
        bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCB(pwText, &dcb);  
        /** enable RTS flow control */  
        dcb.fRtsControl = RTS_CONTROL_ENABLE;  
   
        delete[] pwText;  
    }  
  
    if (bIsSuccess)  
    {  
        /** Set com state with DCB */  
        bIsSuccess = SetCommState(m_hComm, &dcb);  
    }  
  
    /**  Erase com buffer */  
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);  
   
    LeaveCriticalSection(&m_csCommunicationSync);  
  
    return bIsSuccess == TRUE;  
}  
  
bool CSerialPort::InitPort(UINT portNo, const LPDCB& plDCB)  
{  
	/** Open the oprt, DO NOT add protection since there is one in the function body */
    if (!openPort(portNo))  
    {  
        return false;  
    }  
  
    EnterCriticalSection(&m_csCommunicationSync);  
  
    /** Set com state */  
    if (!SetCommState(m_hComm, plDCB))  
    {  
        return false;  
    }  
  
	/**  Erase com buffer */
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);  
   
    LeaveCriticalSection(&m_csCommunicationSync);  
  
    return true;  
}  
  
void CSerialPort::ClosePort()  
{  
    /** Turn off any working port */  
    if (m_hComm != INVALID_HANDLE_VALUE)  
    {  
        CloseHandle(m_hComm);  
        m_hComm = INVALID_HANDLE_VALUE;  
    }  
}  
  
bool CSerialPort::openPort(UINT portNo)  
{  
    EnterCriticalSection(&m_csCommunicationSync);  
  
    /** convert port num to device name */  
    char szPort[50];  
    sprintf_s(szPort, "COM%d", portNo);  
  
    /** open desiginated port */  
    m_hComm = CreateFileA(szPort,  /** 设备名,COM1,COM2等 */  
        GENERIC_READ | GENERIC_WRITE, /** mode, can write and read at the same time */  
        0,                            /** share mode, 0 for not sharing */  
        NULL,                         /** safety, null for default */  
        OPEN_EXISTING,                /** this parameter has to exist, otherwise the action fails */  
        0,  
        0);  
  
    /** if failed, release all resources */  
    if (m_hComm == INVALID_HANDLE_VALUE)  
    {  
        LeaveCriticalSection(&m_csCommunicationSync);  
        return false;  
    }  
   
    LeaveCriticalSection(&m_csCommunicationSync);  
  
    return true;  
}  
  
bool CSerialPort::OpenListenThread()  
{  
    /** check if thread is on */  
    if (m_hListenThread != INVALID_HANDLE_VALUE)  
    {  
        /** Thread is on */  
        return false;  
    }  
  
    s_bExit = false;  
   
    UINT threadId;  
    /** open listen thread */  
    m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &threadId);  
    if (!m_hListenThread)  
    {  
        return false;  
    }  
    if (!SetThreadPriority(m_hListenThread, THREAD_PRIORITY_ABOVE_NORMAL))  
    {  
        return false;  
    }  
  
    return true;  
}  
  
bool CSerialPort::CloseListenTread()  
{  
    if (m_hListenThread != INVALID_HANDLE_VALUE)  
    {  
        /** Inform the thread to exit */  
        s_bExit = true;  
  
        /** Wait to exit*/  
        Sleep(10);  
  
        /** close handle */  
        CloseHandle(m_hListenThread);  
        m_hListenThread = INVALID_HANDLE_VALUE;  
    }  
    return true;  
}  
  
UINT CSerialPort::GetBytesInCOM()  
{  
    DWORD dwError = 0;  
    COMSTAT  comstat;   /** record the device state */  
    memset(&comstat, 0, sizeof(COMSTAT));  
  
    UINT BytesInQue = 0;  
    /** clear com errors before read and write */  
    if (ClearCommError(m_hComm, &dwError, &comstat))  
    {  
        BytesInQue = comstat.cbInQue; /** get bits in buffer */  
    }  
  
    return BytesInQue;  
}  
  
UINT WINAPI CSerialPort::ListenThread(void* pParam)  
{  
    CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);  
  
    // looping inquiry to read data from serial port    
    while (!pSerialPort->s_bExit)  
    {  
        UINT BytesInQue = pSerialPort->GetBytesInCOM();  
        /** if no data, wait a while */  
        if (BytesInQue == 0)  
        {  
            Sleep(SLEEP_TIME_INTERVAL);  
            continue;  
        }  
  
        /** read data in buffer and print it out */  
        char cRecved = 0x00;  
        do  
        {  
            cRecved = 0x00;  
            if (pSerialPort->ReadChar(cRecved) == true)  
            {  
                std::cout << cRecved;  
                continue;  
            }  
        } while (--BytesInQue);  
    }  
  
    return 0;  
}  
  
bool CSerialPort::ReadChar(char &cRecved)  
{  
    BOOL  bResult = TRUE;  
    DWORD BytesRead = 0;  
    if (m_hComm == INVALID_HANDLE_VALUE)  
    {  
        return false;  
    }  
   
    EnterCriticalSection(&m_csCommunicationSync);  
  
    /** read a Byte from buffer */  
    bResult = ReadFile(m_hComm, &cRecved, 1, &BytesRead, NULL);  
    if ((!bResult))  
    {   
        DWORD dwError = GetLastError();  
  
        /** Purge Com */  
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);  
        LeaveCriticalSection(&m_csCommunicationSync);  
  
        return false;  
    }  
  
    LeaveCriticalSection(&m_csCommunicationSync);  
  
    return (BytesRead == 1);  
  
}  
  
bool CSerialPort::WriteData(unsigned char* pData, unsigned int length)  
{  
    BOOL   bResult = TRUE;  
    DWORD  BytesToSend = 0;  
    if (m_hComm == INVALID_HANDLE_VALUE)  
    {  
        return false;  
    }  

    EnterCriticalSection(&m_csCommunicationSync);  
  
    /** write data to com */  
    bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);  
    if (!bResult)  
    {  
        DWORD dwError = GetLastError();  
        /** purge com */  
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);  
        LeaveCriticalSection(&m_csCommunicationSync);  
  
        return false;  
    }  

    LeaveCriticalSection(&m_csCommunicationSync);  
  
    return true;  
}  

