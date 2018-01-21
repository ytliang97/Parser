#include <iostream>
#include <sstream>

int main() {
	
	std::string sRuleNumber = "0";
	std::stringstream sso;
	sso << sRuleNumber;
	int ruleNum;
	sso >> ruleNum;
	std::cout << ruleNum + 2 << std::endl;
	std::cout << sRuleNumber + "2" << std::endl;


	std::string sInitLetter = "1";
    std::stringstream sso1;
    sso1 << sInitLetter;
    int iInitLetter;
    sso1 >> iInitLetter;
    std::cout << iInitLetter << std::endl;
	return 0;
/* stringstream object
	std::cout << "Enter an integer value" << std::endl;
	int input;
	std::cin >> input;
	
	std::stringstream sso;
	sso << input;
	std::string strinput;
	sso >> strinput;
	
	std::cout << "the integer number" << input << std::endl;
	std::cout << "the string number" << strinput << std::endl;
	
	return 0;
*/
}