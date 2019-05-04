#include "../MyLibs/userInterface.h"
#include <sstream>
#include <iostream>

#define TEST_CLASS UserInterfaceTestor
#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>

using namespace boost::unit_test;
using namespace std;

string getStrFromCoutStream(void (*f)(void))// pointer to function as the argument
{
	string inputStr;
	// get cout stream buffer
	ostringstream oss;
	streambuf* p_cout_streambuf = cout.rdbuf();
	cout.rdbuf(oss.rdbuf());

	(*f)();

	std::cout.rdbuf(p_cout_streambuf); // restore

	inputStr = oss.str();
	return inputStr;
}

class UserInterfaceTestor
{
public:
	UserInterfaceTestor()
	{

	}

	/* Test welcome Msg, in which it must includes:
	1. Name: BLITZ (Behavioral Learning In The Zebrafish)
	2. Weblink: https://github.com/Wenlab/BLITZ
	3. License: GNU License
	4. Citation: (Wenbin Yang et al., 2019)
	5. Author: Wenbin Yang @ bysin7@gmail.com
	*/
	void testWelcomeMsg()
	{
		string name = "BLITZ (Behavioral Learning In The Zebrafish)";
		string weblink = "https://github.com/Wenlab/BLITZ";
		string license = "GNU 3.0 License";
		string citation = "Wenbin Yang et al., 2019";
		string author = "Wenbin Yang";
		string email = "bysin7@gmail.com";


		string inputStr = getStrFromCoutStream(showWelcomeMsg);
		

		BOOST_TEST(string::npos != inputStr.find(name),
			"Not found the program name: " + name);
		BOOST_TEST(string::npos != inputStr.find(weblink),
			"Not found the weblink: " + weblink);
		BOOST_TEST(string::npos != inputStr.find(license),
			"Not found the license: " + license);
		BOOST_TEST(string::npos != inputStr.find(citation),
			"Not found the citation: " + citation);
		BOOST_TEST(string::npos != inputStr.find(author),
			"Not found the author: " + author);
		BOOST_TEST(string::npos != inputStr.find(email),
			"Not found the email: " + email);

		cout << "Finished all tests. Press key to exit." << endl;
		getchar();
	}

	/* Test whether received device statuses are correct */
	void testDeviceStatuses()
	{

	}

	
private:
	UserInterface UIobj;
};

test_suite* init_unit_test_suite(int /*argc*/, char* /*argv*/[])
{
	boost::shared_ptr<TEST_CLASS> tester(new TEST_CLASS);

	framework::master_test_suite().
		add(BOOST_TEST_CASE(boost::bind(&TEST_CLASS::testWelcomeMsg, tester)));
	
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