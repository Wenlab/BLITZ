#pragma once

#ifndef TALK2FCAMERA_H_
#define TALK2FCAMERA_H_

#include <windows.h>
#include <iostream>
#include "NIDAQmx.h"
//#include "opencv2/opencv.hpp"

class FCamera
{
private:
	;
public:
	// methord
	FCamera()
	{
		taskHandle = NULL;
	}

	void handleDAQError(int error);
	bool initialize();
	bool startTask();
	int getFromFcamera();

	int32 samps;
	int read_[99];
	TaskHandle taskHandle;
};


#endif /* TALK2FCAMERA_H_ */