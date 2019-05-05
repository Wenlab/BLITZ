#include "../MyLibs/userInterface.h"
#include <sstream>
#include <iostream>
#include <vector>


#define TEST_CLASS UserInterfaceTestor
#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>

using namespace boost::unit_test;
using namespace std;

string getStrFromCoutStream(void (*f)(void))// pointer to function as the argument
{
	string outputStr;
	// get cout stream buffer
	ostringstream oss;
	streambuf* p_cout_streambuf = cout.rdbuf();
	cout.rdbuf(oss.rdbuf());

	(*f)();

	std::cout.rdbuf(p_cout_streambuf); // restore

	outputStr = oss.str();
	return outputStr;
}
/* Adapted from the code snippet on Stackoverflow: 
https://stackoverflow.com/a/3797313 
*/
void sendStr2CinStream(std::istream& is, string inputStr)
{
	// inject 
	streambuf* backup;
	istringstream oss(inputStr);
	backup = is.rdbuf();
	is.rdbuf(oss.rdbuf());

}

class UserInterfaceTestor
{
public:
	UserInterfaceTestor()
	{

	}

	/* Full test for all the functionalities, 
	however, programmer has to check the results in the CMD
	manually 
	*/
	void fullManualTest() //TODO: replace with an automated unit test
	{
		UIobj.enquireInfoFromUser();

		// display all the input in the CMD
		cout << "devices2use: " << endl;
		vector<bool>& devices2use = UIobj.devices2use;
		for (int i = 0; i < devices2use.size(); i++)
			cout << devices2use[i] << endl;

		cout << "cameras2open: " << endl;
		vector<bool>& cameras2open = UIobj.cameras2open;
		for (int i = 0; i < cameras2open.size(); i++)
			cout << cameras2open[i] << endl;

		cout << "allFishIDs: " << endl;
		std::vector<std::vector<int>> allFishIDs = UIobj.allFishIDs;
		for (int i = 0; i < allFishIDs.size(); i++)
		{
			cout << "Arena " << UIobj.arenaIDs[i] << " :" << endl;
			vector<int> fishIDs = allFishIDs[i];
			for (int j = 0; j < fishIDs.size(); j++)
				cout << fishIDs[j] << endl;
		}

		cout << "fishAge: " << endl;
		int fishAge = UIobj.fishAge;
		cout << fishAge << endl;

		cout << "fishStrain: " << endl;
		string strainName = UIobj.strainName;
		cout << strainName << endl;

		cout << "expTask: " << endl;
		string expTask = UIobj.expTask;
		cout << expTask << endl;

		cout << "visPattern: " << endl;
		string visPattern = UIobj.visPattern;
		cout << visPattern << endl;

		cout << "startTimeStr: " << endl;
		string startTimeStr = UIobj.startTimeStr;
		cout << startTimeStr << endl;

		cout << "baseNames: " << endl;
		vector<string> baseNames = UIobj.baseNames;
		for (int i = 0; i < baseNames.size(); i++)
			cout << baseNames[i] << endl;


		cout << endl << "All tests finished. Press key to exit." << endl;
		getchar();
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
		// TODO: use a better method to drop this amendment
		// Currently, the error message cannot be printed in the CMD
		cout << "Finished all tests. Press key to continue/exit." << endl;
		getchar();
		
	}

	/* Test whether received device statuses are correct */
	void testDeviceStatuses() // TODO: reconstruct this test
	{
		// TODO: automatically test the full input space
		string inputStr = "1, 2, 3\n fullBlue\n 0, 1, 1";
		vector<bool> expectedRes = { 1, 1, 1 };

		// TODO: wrap up these lines for injecting string to std::istream
		streambuf* backup;
		istringstream oss(inputStr);
		backup = cin.rdbuf();
		cin.rdbuf(oss.rdbuf());

		UIobj.enquireDevice2use(cin);

		BOOST_CHECK_EQUAL_COLLECTIONS(
			UIobj.devices2use.begin(), UIobj.devices2use.end(),
			expectedRes.begin(), expectedRes.end()
		);
		
		cout << "Finished all tests. Press key to continue/exit." << endl;
		getchar();
		
	}

	/* Test whether received cameras2open statuses are correct */
	void testCameras2use()
	{

		string inputStr = "0, 1, 1";
		vector<bool> expectedRes = { 0, 1, 1 };

		// TODO: wrap up these lines for injecting string to std::istream
		streambuf* backup;
		istringstream oss(inputStr);
		backup = cin.rdbuf();
		cin.rdbuf(oss.rdbuf());

		UIobj.enquireCameras2use(cin);

		BOOST_CHECK_EQUAL_COLLECTIONS(
			UIobj.cameras2open.begin(), UIobj.cameras2open.end(),
			expectedRes.begin(), expectedRes.end()
		);

		cout << "Finished all tests. Press key to continue/exit." << endl;
		getchar();
	}

	/* Test whether receiving correct visual pattern */
	void testVisPattern()
	{
		string inputStr = "fullBlue";
		// TODO: wrap up these lines for injecting string to std::istream
		streambuf* backup;
		istringstream oss(inputStr);
		backup = cin.rdbuf();
		cin.rdbuf(oss.rdbuf());

		UIobj.enquirePattern2use(cin);
		
		// TODO: this seems to be a trivial test
		BOOST_CHECK_EQUAL(UIobj.visPattern, inputStr);
		
		cout << "Finished all tests. Press key to continue/exit." << endl;
		getchar();
	
	}

	/* Test whether receiving correct fishStrain */
	void testFishStrain()
	{
		string inputStr = "GCaMP6S";
		// TODO: wrap up these lines for injecting string to std::istream
		streambuf* backup;
		istringstream oss(inputStr);
		backup = cin.rdbuf();
		cin.rdbuf(oss.rdbuf());

		UIobj.enquireFishStrain(cin);

		// TODO: this seems to be a trivial test
		BOOST_CHECK_EQUAL(UIobj.strainName, inputStr);

		cout << "Finished all tests. Press key to continue/exit." << endl;
		getchar();

	}

	/* Test whether receiving correct fish age */
	void testFishAge()
	{
		string inputStr = "7";
		// TODO: wrap up these lines for injecting string to std::istream
		streambuf* backup;
		istringstream oss(inputStr);
		backup = cin.rdbuf();
		cin.rdbuf(oss.rdbuf());

		UIobj.enquireFishAge(cin);

		// TODO: this seems to be a trivial test
		BOOST_CHECK_EQUAL(UIobj.fishAge, stoi(inputStr));

		cout << "Finished all tests. Press key to continue/exit." << endl;
		getchar();

	}

	/* Test generated basename */
	void testBasename()
	{

	}
	
private:
	UserInterface UIobj;
};

test_suite* init_unit_test_suite(int /*argc*/, char* /*argv*/[])
{
	boost::shared_ptr<TEST_CLASS> tester(new TEST_CLASS);
	/*
	framework::master_test_suite().
		add(BOOST_TEST_CASE(boost::bind(&TEST_CLASS::testWelcomeMsg, tester)));
	
	framework::master_test_suite().
		add(BOOST_TEST_CASE(boost::bind(&TEST_CLASS::testDeviceStatuses, tester)));
	
	framework::master_test_suite().
		add(BOOST_TEST_CASE(boost::bind(&TEST_CLASS::testCameras2use, tester)));

	framework::master_test_suite().
		add(BOOST_TEST_CASE(boost::bind(&TEST_CLASS::testVisPattern, tester)));

	framework::master_test_suite().
		add(BOOST_TEST_CASE(boost::bind(&TEST_CLASS::testFishStrain, tester)));
	*/
	framework::master_test_suite().
		add(BOOST_TEST_CASE(boost::bind(&TEST_CLASS::testFishAge, tester)));

	
	
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