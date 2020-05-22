#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h> 
#include <iostream>


#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "talk2camera.h"
#include "prepare.h"
#include "talk2screen.h"
#include "fishAnalysis.h"
#include "talk2relay.h"
#include "C:\Users\USER\source\repos\rewrite-learning\rewrite-learning\fileWriter.h"

using namespace std;
using namespace cv;





// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;







int main() {
	//test talk2screen
	vector<float> a = { -0.0F,-0.5F };

	vector<float> b = { 0.5F,0.0F };
	bool lefttrue = true;
	ScreenData test(a, b, lefttrue);
	test.ScreenInitialize();

	test.PatternInitialize();

	//test.reversePattern();

	while (1) {
		test.renderTexture();

	}


	/*
	Mat cur_img;
	VideoCapture cap("F://fishData//test5.avi");
	int relay_com_num = 3;
	int checkPoint = 0;
	
	Relay relayObj;
	Prepare prepareObj;
	Screen screenObj;
	ExpTimer timerObj;
	FileWriter fileWriterObj;
	Camera cameraObj;

	relayObj.initialize(relay_com_num);
	screenObj.initialize();
	cameraObj.initialize();
	fileWriterObj.initialize("test", cameraObj.getFrameSize());

	cap >> cur_img;
	prepareObj.pretreat(cur_img);
	prepareObj.convertPatchPoint();
	prepareObj.setHeadAndCenter();
	prepareObj.setLeftPatch(screenObj.leftPatch);
	prepareObj.setRightPatch(screenObj.rightPatch);
	screenObj.bindVBOandVAO();

	FixedFish fixedfishObj(prepareObj.getHead(), prepareObj.getCenter(), prepareObj.getThreshold_val(), prepareObj.getMask());

	for (; fixedfishObj.count < 40; fixedfishObj.addCount()) {
		fixedfishObj.getImgFromVideo(cap);
		fixedfishObj.fishAngleAnalysis(fixedfishObj.count);

		if (checkPoint == 1) {
			checkPoint--;
			fixedfishObj.predictDirection();
		}
		if (checkPoint > 0) {
			checkPoint--;
		}

		if (checkPoint == 0 && fixedfishObj.tailingFlag[fixedfishObj.count]) {
			checkPoint = fixedfishObj.boutFrames;
		}
		//TODO:shockon or not
		fileWriterObj.writeOutFrame(timerObj, fixedfishObj);
	}

	*/

	//cout << "finished" << endl;
	//waitKey();

	//Camera cameraObj;
	//cameraObj.initialize();
	//
	//cameraObj.grabPylonImg();
	////cur_img=cameraObj.getMat();
	//cap >> cur_img;
	//Prepare prepareObj;

	//prepareObj.pretreat(cur_img);

	//prepareObj.convertPatchPoint();
	//prepareObj.setHeadAndCenter();


	//Screen screenObj;
	//screenObj.initialize();

	//prepareObj.getLeftPatch(screenObj.leftPatch);
	//prepareObj.getRightPatch(screenObj.rightPatch);
	//
	//
	//

	//
	//screenObj.bindVBOandVAO();

	//screenObj.showLeftPatch();
	//
	//screenObj.showRightPatch();
	//
	//FixedFish fixedfishObj(prepareObj.getHead(), prepareObj.getCenter(), prepareObj.getThreshold_val());
	//

	////fixedfishObj.getImgFromVideo(cap);


	return 0;


}



