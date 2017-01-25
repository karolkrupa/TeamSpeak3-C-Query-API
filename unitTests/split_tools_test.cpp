#include <Server.hpp>
#include <iostream>
#include <map>

using namespace std;
using namespace Ts3Api;

void testError(string msg) {
	cout << msg << endl;
	exit(1);
}

void success() {
	exit(0);
}

void test1() {
	cout << "---- TEST 1 ----" << endl;
	string firstTest = "arg1=1 arg2=2 arg3=3";

	map<string, string> firstTestMap;
	split(firstTestMap, firstTest);

	if(firstTestMap.size() != 3) testError("Step 1");

	if(firstTestMap["arg1"] != "1") testError("Step 2");
	if(firstTestMap["arg2"] != "2") testError("Step 3");
	if(firstTestMap["arg3"] != "3") testError("Step 4");
}

void test2() {
	cout << "---- TEST 2 ----" << endl;
	string firstTest = "arg1=1 arg2 arg3=";

	map<string, string> firstTestMap;
	split(firstTestMap, firstTest);

	if(firstTestMap.size() != 3) testError("Step 1");

	if(firstTestMap["arg1"] != "1") testError("Step 2");
	if(firstTestMap["arg2"] != "unknown") testError("Step 3");
	if(firstTestMap["arg3"] != "unknown") testError("Step 4");
}

void test3() {
	cout << "---- TEST 3 ----" << endl;
	string testStr = "arg1=1 arg2=2 arg3=3 uid=1|uid=2 arg1=1 arg2=2 arg3=3";
	map<string, map<string, string>> testMap;

	split(testMap, testStr, "uid");

	if(testMap.size() != 2) testError("Step 1");

	if(testMap["1"]["arg1"] != "1") testError("Step 2");
	if(testMap["1"]["arg2"] != "2") testError("Step 3");
	if(testMap["1"]["arg3"] != "3") testError("Step 4");
	if(testMap["1"]["uid"] != "1") testError("Step 5");

	if(testMap["2"]["arg1"] != "1") testError("Step 6");
	if(testMap["2"]["arg2"] != "2") testError("Step 7");
	if(testMap["2"]["arg3"] != "3") testError("Step 8");
	if(testMap["2"]["uid"] != "2") testError("Step 9");
}

void test4() {
	cout << "---- TEST 4 ----" << endl;
	string testStr = "arg1=1 arg2=2 arg3=3 uid=1|uid=2 arg1=1 arg2=2 arg3=3";
	map<string, map<string, string>> testMap;

	split(testMap, testStr);

	if(testMap.size() != 2) testError("Step 1");

	if(testMap["0"]["arg1"] != "1") testError("Step 2");
	if(testMap["0"]["arg2"] != "2") testError("Step 3");
	if(testMap["0"]["arg3"] != "3") testError("Step 4");
	if(testMap["0"]["uid"] != "1") testError("Step 5");

	if(testMap["1"]["arg1"] != "1") testError("Step 6");
	if(testMap["1"]["arg2"] != "2") testError("Step 7");
	if(testMap["1"]["arg3"] != "3") testError("Step 8");
	if(testMap["1"]["uid"] != "2") testError("Step 9");
}



int main() {
	test1();
	test2();
	test3();
	test4();
	

	cout << "SUCCESS" << endl;
	return 0;
}