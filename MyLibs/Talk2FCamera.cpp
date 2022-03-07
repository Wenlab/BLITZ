#include <stdio.h>
#include <math.h>
#include <windows.h>

#include "Talk2FCamera.h"


void FCamera::handleDAQError(int error)
{
	char  errBuff[2048] = { '\0' };

	if (DAQmxFailed(error))
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	if (taskHandle != 0) {
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if (DAQmxFailed(error))
		printf("DAQmx Error: %s\n", errBuff);
	printf("End of program, press Enter key to quit\n");
	getchar();
}

bool FCamera::initialize()
{
	int error = 0;

	error = DAQmxCreateTask("", &taskHandle);

	if (error != 0)
	{
		handleDAQError(error);
		printf("ERROR! Cannot control the stage with DAQ card.\n");
		taskHandle = NULL;
		return false;
	}
	error = DAQmxCreateDIChan(taskHandle, "Dev1/port0/line5", "", DAQmx_Val_ChanPerLine);
	if (error != 0)
	{
		handleDAQError(error);
		printf("ERROR! Cannot control the stage with DAQ card.\n");
		taskHandle = NULL;
		return false;
	}
	error = DAQmxCfgSampClkTiming(taskHandle, NULL, 1000, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 10000);
	//error = DAQmxCfgBurstHandshakingTimingExportClock(taskHandle, DAQmx_Val_ContSamps, 10000, 100, "", DAQmx_Val_Rising, int32 pauseWhen, int32 readyEventActiveLevel);;
	if (error != 0)
	{
		handleDAQError(error);
		printf("ERROR! Cannot control the stage with DAQ card.\n");
		taskHandle = NULL;
		return false;
	}
	return true;
}

bool FCamera::startTask()
{
	int error = 0;

	error = DAQmxStartTask(taskHandle);

	if (error != 0)
	{
		handleDAQError(error);
		printf("ERROR! Connecting to the PCIe failed.\n");
		return false;
	}
	return true;
}

int FCamera::getFromFcamera()
{
	//int32 written;
	int	error = 0;


	
	//read_ = { 0,0,0,0,0,0,0,0,0,0 };
	uInt8 read[100];

	error = DAQmxReadDigitalLines(taskHandle,-1,-1,DAQmx_Val_GroupByScanNumber,read,100, &samps,NULL,NULL);


	if (error != 0)
	{
		handleDAQError(error);
		printf("ERROR! cannot send speed control commands.\n");
		return -1;
	}
	
	std::cout << samps << std::endl;
	for (int i = 0; i < 99; i++) {

		read_[i] = (unsigned)read[i];
		std::cout << read_[i];
		
	}
	std::cout <<  std::endl;
	return 0;
}