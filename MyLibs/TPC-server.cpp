#include "TCP-server.h"

void TCP_server::initialize() {


	server_ = INVALID_SOCKET;
	int result = 0;
	result = WSAStartup(MAKEWORD(2, 2), &data_);
	if (result != 0)
	{
		std::cout << "WSAStartup() init error " << GetLastError() << std::endl;
		system("pause");
	}

	server_ = socket(AF_INET, SOCK_STREAM, 0);

	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(11000);
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	result = bind(server_, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
	if (result != 0)
	{
		std::cout << "bind() error" << result;
		system("pause");

	}
	result = listen(server_, 10);
	if (result != 0)
	{
		std::cout << "listen error" << result;
		system("pause");

	}

	int len = sizeof(SOCKADDR);


	std::cout << "wait  moving stage connect......" << std::endl;
	socketStage = accept(server_, (SOCKADDR*)&addrClient, &len);

	std::cout << "wait Excitation connect......" << std::endl;
	socketExcitation = accept(server_, (SOCKADDR*)&addrClient, &len);

	std::cout << "wait  behavior camera connect......" << std::endl;
	socketCamera1 = accept(server_, (SOCKADDR*)&addrClient, &len);


	if (socketStage == SOCKET_ERROR)
	{
		std::cout << " moving stage connect error" << WSAGetLastError();

	}
	if (socketExcitation == SOCKET_ERROR)
	{
		std::cout << " Excitation connect error" << WSAGetLastError();

	}
	if (socketCamera1 == SOCKET_ERROR)
	{
		std::cout << " Excitation connect error" << WSAGetLastError();

	}
	char str[INET_ADDRSTRLEN];

	std::cout<< "Server get connection with client socket : [" << inet_ntop(AF_INET, &addrClient.sin_addr, str, sizeof(str)) << ":" << ntohs(addrClient.sin_port) << "]" << std::endl;
	
}
void TCP_server::getLocation() {
	
	memset(recvBuff, 0, sizeof(recvBuff));
	recv(socketStage, recvBuff, sizeof(recvBuff), 0);
	//std::cout << "client say:" << recvBuff << std::endl;
	platformY = 0;
	for (int i = 1; i < 7; i++) {
		if (recvBuff[std::strlen(recvBuff) - i] == ' '|| recvBuff[std::strlen(recvBuff) - i] == '-')
			break;
		platformY += (recvBuff[std::strlen(recvBuff) - i] - '0') * (pow(10,i-1));
		//std::cout << platformY << std::endl;
	}
	platformX = 0;
	for (int i = 7; i < 13; i++) {
		if (recvBuff[std::strlen(recvBuff) - i] == ' ' || recvBuff[std::strlen(recvBuff) - i] == '-')
			break;
		platformX += (recvBuff[std::strlen(recvBuff) - i] - '0') * (pow(10,i-7));
		//std::cout << platformX << std::endl;
	}
	screenX = int(148.2 + (0.0007467)*platformX + (6.765E-5)*platformY);

	screenY = int(236.8 + (6.115E-5)*platformX + (-0.0005627)*platformY);
	if (screenX < 0 || screenX>500 || screenY < 0 || screenY>500) {
		screenX = 51;
		screenY = 51;
	}
	//std::cout << "screenX:" << screenX << std::endl;
	//std::cout << "screenY:" << screenY << std::endl;

	//fishLocaton?

	//platformX = (recvBuff[std::strlen(recvBuff) - 1] - '0') + (recvBuff[std::strlen(recvBuff) - 2] - '0') * 10 + (recvBuff[std::strlen(recvBuff) - 3] - '0') * 100;

}
//void TCP_server::getLocation_test() {
//
//
//	platformX = *mem;
//	platformY = *mem2;
//	HeadX = 100-platformX;
//	HeadY = 100-platformY;
//
//}
std::string TCP_server::setExcitation_test(int pattern) {
	
	switch (pattern) {
	case 2:
		return "LF"; 
	case 3:
		return "HF";
	}
		
}

void TCP_server::setExcitation(int pattern) {

	switch (pattern) {
	case 2:
		send(socketExcitation, LF, sizeof(LF), 0);
		break;
	case 3:
		send(socketExcitation, HF, sizeof(HF), 0);
		break;
	}
}

void TCP_server::talk2ICamera(int FrameNum) {
	int ii=0;
	int FrameNumSended = FrameNum;
	for (int i = 0; i < 6; i++) {
		ii = FrameNumSended % 10;
		FrameNumSended = FrameNumSended / 10;
		Framenum[6-i-1] = ii + '0';
	}
	//std::cout << Framenum << std::endl;
	sendto(server_UDP, Framenum, sizeof(Framenum), 0,(struct sockaddr*)&addrClient_UDP, len);
}

void TCP_server::recvFromICamera() {
	char recvBuff[100] = { 0 };
	recvfrom(server_UDP, recvBuff, sizeof(recvBuff), 0,(SOCKADDR*)&addrClient_UDP,&len);
	std::cout << "Camera1:"<<recvBuff << std::endl;
	char client_addr[20];
	inet_ntop(AF_INET, &addrClient_UDP.sin_addr.S_un.S_addr, client_addr, 20);
	std::cout << "Camera1_addr:" << client_addr << std::endl;
	//sendto(server_, Framenum, sizeof(Framenum), 0, (struct sockaddr*)&addrClient, len);
}
void TCP_server::initializeForICamera() {

	server_ = INVALID_SOCKET;
	int result = 0;
	result = WSAStartup(MAKEWORD(2, 2), &data_);
	if (result != 0)
	{
		std::cout << "WSAStartup() init error " << GetLastError() << std::endl;
		system("pause");
	}

	server_ = socket(AF_INET, SOCK_STREAM, 0);
	server_UDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(11000);
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	addrSrv_UDP.sin_family = AF_INET;
	addrSrv_UDP.sin_port = htons(11100);
	addrSrv_UDP.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	char IP[] = "192.168.1.30";
	addrClient_UDP.sin_family = AF_INET;
	addrClient_UDP.sin_port = htons(11100);
	inet_pton(AF_INET, IP, &addrClient_UDP.sin_addr.S_un.S_addr);

	result = bind(server_, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR));
	bind(server_UDP, (LPSOCKADDR)&addrSrv_UDP, sizeof(SOCKADDR));
	if (result != 0)
	{
		std::cout << "bind() error" << result;
		system("pause");

	}
	result = listen(server_, 10);
	if (result != 0)
	{
		std::cout << "listen error" << result;
		system("pause");

	}

	std::cout << "wait  moving stage connect......" << std::endl;
	socketStage = accept(server_, (SOCKADDR*)&addrClient, &len);

	std::cout << "wait Excitation connect......" << std::endl;
	socketExcitation = accept(server_, (SOCKADDR*)&addrClient, &len);


	if (socketStage == SOCKET_ERROR)
	{
		std::cout << " moving stage connect error" << WSAGetLastError();

	}
	if (socketExcitation == SOCKET_ERROR)
	{
		std::cout << " Excitation connect error" << WSAGetLastError();

	}

	char str[INET_ADDRSTRLEN];

	std::cout << "Server get connection with client socket : [" << inet_ntop(AF_INET, &addrClient.sin_addr, str, sizeof(str)) << ":" << ntohs(addrClient.sin_port) << "]" << std::endl;

}

void TCP_server::closeExcitation() {

	
	send(socketExcitation, END, sizeof(END), 0);


}