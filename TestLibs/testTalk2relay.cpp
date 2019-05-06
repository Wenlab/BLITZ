#include "../MyLibs/talk2relay.h"
#include <sstream>
#include <iostream>
#include <vector>


#define TEST_CLASS RelayTestor
#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>

using namespace boost::unit_test;
using namespace std;

class RelayTestor
{
public:
	RelayTestor()
	{

	}

	/* Full test for all the functionalities,
	however, programmer has to check the results in the CMD
	manually
	*/
	void fullManualTest() //TODO: replace with an automated unit test
	{
		


		cout << endl << "All tests finished. Press key to exit." << endl;
		getchar();
	}

	/* Test if the generated commands are correct with given input info */
	void testCommandGeneration()
	{
		vector<bool> channelStatuses = 
		{ 1,0,1,0,1,0,1,0,
		  0,1,0,1,0,1,0,1 
		};

		float openDuration = 0.3;
		unsigned char expectedRes[] = {0x00, 0x5A, 0x60, 0x01, 0x12, 0x55, 0xAA, 0x03, 0xC0};

		unsigned char* openCommand = rObj.generateOpenCommand(channelStatuses, openDuration);
		BOOST_TEST(openCommand == expectedRes);

	}

	

private:
	Relay rObj;
};

test_suite* init_unit_test_suite(int /*argc*/, char* /*argv*/[])
{
	boost::shared_ptr<TEST_CLASS> tester(new TEST_CLASS);

	framework::master_test_suite().
		add(BOOST_TEST_CASE(boost::bind(&TEST_CLASS::testCommandGeneration, tester)));



	return 0;
}



/*
BOOST_AUTO_TEST_CASE(test_macro_overview)
{
namespace tt = boost::test_tools;
int a = 1;
int b = 2;
//BOOST_TEST(a != b - 1);
//BOOST_TEST(a + 1 < b);
BOOST_TEST(b - 1 > a, a << " < " << b - 1 << " does not hold");
//BOOST_TEST(a == b, tt::bitwise());
//BOOST_TEST(a + 0.1 == b - 0.8, tt::tolerance(0.01));
getchar();
}
*/