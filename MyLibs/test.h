
#ifndef _GUARD_TEST_H
#define _GUARD_TEST_H

// Include OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>

// Include standard libraries
#include <vector>

#include <Python.h>

#include "fishAnalysis.h"
#include "fileWriter.h"

class test {

private:
	;
public:
	// methods
	test(int xDivide = 0, std::string fishID = "") // constructor
	{

	}

	void creatFrames(cv::Scalar stripe = cv::Scalar(255, 255, 255),
		cv::Scalar background = cv::Scalar(0, 0, 0),
		int wid = 80,
		int gap = 50,
		int angle = 30
	);

	void showFrame(int go=1);

	void findFishTail();

	void findFishTail(std::string path);

	// properties

	std::vector<cv::Mat> Frames;
	int iter=0;

	int wait = 1;
	int totalFrames;
	//Arena ArenaObj(1); // object that processes images
};





#endif // !_GUARD_TEST_H
