#include "../MyLibs/talk2camera.h"
#include <sstream>
#include <iostream>
#include <vector>


#define TEST_CLASS SingleCameraTestor
#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>

using namespace boost::unit_test;
using namespace std;

class SingleCameraTestor
{
public:
	SingleCameraTestor()
	{

	}

	/* Simple test for the basic function */
	void basicManualTest()
	{
		cObj.initialize();
		int numFrames2grab = 1000;

		// giant grabbing loop
		for (int i = 0; i < numFrames2grab; i++)
		{

			cObj.grabPylonImg();
#ifdef PYLON_WIN_BUILD
			// Display the grabbed image.
			Pylon::DisplayImage(1, cObj.pylonImg);
#endif

		}
	}

private:
	SingleCamera cObj;
	

};


class MultiUSBCamerasTestor
{
public:
	MultiUSBCamerasTestor()
	{

	}

	/* Simple test for the basic function */
	void basicManualTest()
	{
		// TODO: write the implementation
	}

private:
	MultiUSBCameras multiCobj;
};