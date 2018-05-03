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
  
#ifndef SERIALPORT_H_    
#define SERIALPORT_H_    
  
#include <Windows.h>    
#include <tchar.h>
#include <stdio.h>
#include <SDKDDKVer.h>
  
/** Serial Port Class 
* 
* Implement the basic operations such as read and write to serial port.
*/  

class CSerialPort  
{  
public:  
    CSerialPort(void);  
    ~CSerialPort(void);  
  
public:  
  
    /** Initialization
    * 
    *  @param:  UINT portNo COM 1 as default, better not to exceed 9 
    *  @param:  UINT baud   baud rate, 9600 as default 
    *  @param:  char parity whether to run odd-even check, Y for yes, N for no. 
    *  @param:  UINT databits 8 as default
    *  @param:  UINT stopsbits 1 as default 
    *  @param:  DWORD dwCommEvents EV_RXCHAR as default, an event occurs whenever a char sent or received 
    *  @return: bool  check if initialization succeeds
    *  @note:   
	*	1.Initialize before any call of functions, 
	*   2.One can use overload functions to set DCB parameters if necessary
    *　 3.The port will be closed automatically, no need for extra commands 　　    
    *         
    *  @see: 
    */  
    bool InitPort(UINT  portNo = 1, UINT  baud = CBR_9600, char  parity = 'N', UINT  databits = 8, UINT  stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR);  
  
    /** to initialize the port
    * 
    *  the function provides arguments to initialize com port by CDB
    *  @param:  UINT portNo 
    *  @param:  const LPDCB & plDCB 
    *  @return: bool  check if initialization succeeds
    *  @note:   the function provides arguments to initialize com port 
    *  @see: 
    */  
    bool InitPort(UINT  portNo, const LPDCB& plDCB);  
  
    /** open listening thread
    * 
    *  receive data to the port and print them out to the screen 
    *  @return: bool  check if succeeds
    *  @note:   return false when the port is on
    *  @see: 
    */  
    bool OpenListenThread();  
  
    /** close the listening
    * 
    * 
    *  @return: bool  check if succeeds 
    *  @note:   the listening thread would be shut down after the call
    *  @see: 
    */  
    bool CloseListenTread();  
  
    /** write to the port 
    * 
    *  write data to the port
    *  @param:  unsigned char * pData, pointer to data in buffer 
    *  @param:  unsigned int length, length of data 
    *  @return: bool  check if succeeds
    *  @note:   do not set the length larger than the buffer size
    *  @see: 
    */  
    bool WriteData(unsigned char* pData, unsigned int length);  
  
    /** get bytes in com
    * 
    * 
    *  @return: UINT  check if succeeds
    *  @note:   return 0 when no data 
    *  @see: 
    */  
    UINT GetBytesInCOM();  
  
    /** read a byte form the port
    * 
    * 
    *  @param:  char & cRecved, variable to store the data
    *  @return: bool  check if succeeds
    *  @note: 
    *  @see: 
    */  
    bool ReadChar(char &cRecved);  
  
private:  
  
    /** open the port
    * 
    * 
    *  @param:  UINT portNo port number 
    *  @return: bool  check if succeeds 
    *  @note: 
    *  @see: 
    */  
    bool openPort(UINT  portNo);  
  
    /** close the port 
    * 
    * 
    *  @return: void  check if succeeds
    *  @note: 
    *  @see: 
    */  
    void ClosePort();  
  
    /** listening thread 
    * 
    *  listen the data to the port
    *  @param:  void * pParam, thread parameters 
    *  @return: UINT WINAPI, thread return value 
    *  @note: 
    *  @see: 
    */  
    static UINT WINAPI ListenThread(void* pParam);  
  
private:  
  
    /** handle to com */  
    HANDLE  m_hComm;  
  
    /** flag to exit */  
    static bool s_bExit;  
  
    /** thread handle */  
    volatile HANDLE    m_hListenThread;  
  
    /** critical section protection */  
    CRITICAL_SECTION   m_csCommunicationSync;       //!< conflict com operations    
  
};  
  
#endif //SERIALPORT_H_   