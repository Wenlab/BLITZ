//#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
//#include <WinSock2.h>
//#include <Ws2tcpip.h>
//#pragma comment(lib,"ws2_32.lib")
#include <string>
#include "talk2screen_cv.h"
#include "TCP-server.h"
#include "experiment.h"
#include "talk2relay.h"
#include "talk2FCamera.h"
#include "expTimer.h"
#include "fileWriter.h"

int main()
{
/*
	FCamera testgalvo;
	TCP_server test_tcp;
	saveData testsave;
	ExpTimer testtimer;
	test_tcp.initializeForICamera();*/
	//getchar();
	/*while (1) {
		test_tcp.recvFromCamera1();
		Sleep(50);
	}*/
	/*
	testtimer.start();
	testgalvo.initialize();
	testgalvo.startTask();
	std::string name = "test1";
	testsave.test_ini(800, name);
*/
	//int i = 0;
	//while (i<10000) {
	//	testgalvo.spinGalvo();
	//	Sleep(50);
	//	cout << i << endl;
	//	i++;
	//}


	 

	//int i = 0;
	//while (i < 10000) {
	//	//	if ((i / 100) % 2 == 1) {
	//	//		test_tcp.setExcitation(2);
	//	//	}
	//	//	else {
	//	//		test_tcp.setExcitation(3);
	//	//	}

	//	test_tcp.talk2camera1(i);
	//	Sleep(50);
	//	i++;
	//}


	//ofstream outfile;
	//outfile.open("F:/FishExpData/data1.csv", ios::out);


	//int i;
	//for (testtimer.resetCount(); testtimer.getCount() < 800; testtimer.addCount()) {
	//	i = testtimer.getCount();

	//	if ((i / 100) % 2 == 1) {
	//		test_tcp.setExcitation(2);
	//	}
	//	else {
	//		test_tcp.setExcitation(3);
	//	}
	//	test_tcp.talk2ICamera(i);
	//	testgalvo.getFromFcamera();
	//	testsave.test_frame(testtimer, testgalvo, i);
	//	/*for (int j = 0; j < 99; j++) {
	//		outfile << testgalvo.read_[j] << ',';
	//	}
	//	outfile << testgalvo.samps << '\n';
	//	*/
	//	while (1) {
	//		testtimer.getTime();
	//		if ((testtimer.sElapsed * 1000 + testtimer.msRemElapsed) > testtimer.getCount() * 50)
	//			break;
	//	}

	//}

	//test_tcp.closeExcitation();
	//testsave.test_save(name);





	Screen test_screen;
	test_screen.test();
	/*Relay testrelay;

	testrelay.initialize(3);
	vector<bool> channels2open = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	while (1) {
		testrelay.givePulse(channels2open,0.3);
		Sleep(5000);
	}
	testrelay.givePulse(channels2open);*/



	//while (1) {
	//	test_tcp.getLocation();
	//	std::cout <<"platformY:" <<test_tcp.platformY << endl;
	//	std::cout << "platformX:" << test_tcp.platformX << endl;
	//	//test_tcp.setExcitation(2);
	//	Sleep(500);
	//}
	//for (int i=0; 1;i++) {
	//	test_tcp.recvFromCamera1();
	//	
	//	std::cout << i << std::endl;


	//	Sleep(1000);
	//}



	//test_tcp.closeExcitation();

	//test_tcp.getLocation_test();

	//Screen test1;
	//test1.initialize(2);
	//Fish fishObj;
	//while (1) {
	//	test_tcp.getLocation();
	//	std::cout <<"platformY:" <<test_tcp.platformY << endl;
	//	std::cout << "platformX:" << test_tcp.platformX << endl;
	//	test1.refresh(1, "test2", test_tcp.screenX, test_tcp.screenY);
	//	fishObj.getPosition(test_tcp.platformX, test_tcp.platformY);
	//	
	//	std::cout << "fishObj.head.x:" << fishObj.head.x << endl;
	//	std::cout << "fishObj.head.y:" << fishObj.head.y << endl;
	//	//cv::waitKey();
	//	Sleep(500);
	//}
	/*Experiment test;

	test.initializeTest_3();
	test.runOLexptest_3();*/

	//test1.refresh(1, "test2", 20, 100);
	//cv::waitKey();
	//test1.refresh(1, "test", 100, 20);
	//cv::waitKey();
	//test1.refresh(1, "test", 200, 20);
	//cv::waitKey();
}
