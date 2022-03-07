#ifndef _TCP_SERVER_DEF
#define _TCP_SERVER_DEF



#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <Ws2tcpip.h>
#include <cmath>
#pragma comment(lib,"ws2_32.lib")

//#include <boost/lexical_cast.hpp>       
//#include <iostream>
//#include <boost/interprocess/shared_memory_object.hpp>
//#include <boost/interprocess/mapped_region.hpp>

class TCP_server
{
public:
	TCP_server() {

	}

	void initialize();

	void getLocation();
	//void getLocation_test();
	std::string setExcitation_test(int pattern);
	//void initialize_test();
	void setExcitation(int pattern);
	void talk2ICamera(int FrameNum);
	void closeExcitation();
	void initializeForICamera();
	void recvFromICamera();

	WSADATA data_;
	SOCKADDR_IN addrSrv;
	SOCKADDR_IN addrSrv_UDP;
	SOCKADDR_IN addrClient;
	SOCKADDR_IN addrClient_UDP;
	//SOCKET socketClient;
	SOCKET server_;
	SOCKET server_UDP;
	SOCKET socketStage;
	SOCKET socketExcitation;
	SOCKET socketCamera1;
	SOCKET socketCamera2;

	char recvBuff[1024];
	char HF[2] = "0";
	char LF[2] = "1";
	char END[2] = "2";
	char Framenum[7] = "000000";
	int HeadX=200;
	int HeadY=200;
	int platformX=0;
	int platformY=0;
	int screenX, screenY;
	int len = sizeof(SOCKADDR);

	//HANDLE hShareMem = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4, L"SharedMemory");
	//HANDLE hShareMem2 = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4, L"SharedMemory2");
	//int* mem = static_cast<int*>(MapViewOfFile(hShareMem, FILE_MAP_ALL_ACCESS, 0, 0, 4));
	//int* mem2 = static_cast<int*>(MapViewOfFile(hShareMem2, FILE_MAP_ALL_ACCESS, 0, 0, 4));

};





#endif // !_TCP_SERVER_DEF