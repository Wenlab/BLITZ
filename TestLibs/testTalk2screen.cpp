#include "../MyLibs/talk2screen.h"
#include <sstream>
#include <iostream>
#include <vector>


#define TEST_CLASS ScreenTestor
#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>

using namespace boost::unit_test;
using namespace std;

class ScreenTestor
{
public:
	ScreenTestor()
	{

	}

	/* Simple test for the basic function */
	void basicManualTest()
	{
		sObj.initialize(
			"Images/RBC.jpg"
			, "half"
			, { 0.068f, 0.300f, 0.258f, 0.668f }
		);

		while (1)
		{
			sObj.show();
		}
	}

private:
	Screen sObj;
};