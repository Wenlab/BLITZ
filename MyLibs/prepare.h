#ifndef _GUARD_PREPARE_H
#define _GUARD_PREPARE_H
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h> 
#include <iostream>
#include <vector>

using namespace std;










class Prepare
{
public:
	Prepare()
	{
		Center = cv::Point(0, 0);
		Head = cv::Point(0, 0);
		
	}
	void pretreat(cv::Mat curImg);
	void convertPatchPoint();
	void setHeadAndCenter();
	cv::Point getHead();
	cv::Point getCenter();
	cv::Mat getMask();
	int getThreshold_val();
	void setLeftPatch(float *leftPatch);
	void setRightPatch(float* rightPatch);

	int fishAge=7;
	std::string strainName="AB";
	std::string expTask="OLcontrol"; // task to run for animals

	cv::Point Center, Head;
	


private:

	
	float vertices1[9] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
	};
	float vertices2[9] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
	};
	
};



void copyMat(cv::Mat curImg);
static void on_trackbar_setThreshold(int, void*);
bool setThreshold();
void on_mouse_findHeadAndCenter(int event, int x, int y, int flags, void* ustc);
bool findHeadAndCenter();
void on_mouse_setPatch(int event, int x, int y, int flags, void* ustc);
bool setPatch();
void on_mouse_setROI(int event, int x, int y, int flags, void* ustc);
bool setROI();



#endif // !_GUARD_PREPARE_H