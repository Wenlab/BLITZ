#include "../MyLibs/userInterface.h"
#include <sstream>
#include <iostream>

#define TEST_CLASS UserInterfaceTestor
#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>

using namespace boost::unit_test;
using namespace std;

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

		// get cout stream buffer
		std::ostringstream oss;
		std::streambuf* p_cout_streambuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		showWelcomeMsg();

		std::cout.rdbuf(p_cout_streambuf); // restore

		BOOST_TEST(string::npos != oss.str().find(name),
			"Program name not found!");
		BOOST_TEST(string::npos != oss.str().find(weblink),
			"Weblink not found!");
		BOOST_TEST(string::npos != oss.str().find(license),
			"GNU 3.0 License not found!");
		BOOST_TEST(string::npos != oss.str().find(citation),
			"Citation not found!");
		BOOST_TEST(string::npos != oss.str().find(author),
			"Author not found!");
		BOOST_TEST(string::npos != oss.str().find(email),
			"Author email not found!");

		cout << "Finished all tests. Press key to exit." << endl;
		getchar();
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