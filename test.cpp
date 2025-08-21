#include <iostream>


int main()
{
	std::string s1("1234\r");
	std::string s2("1234\n");
	std::string s3("1234\r\n");
	std::cout << "s1 : " << s1.find("\r\n") << std::endl;
	std::cout << "s2 : " << s2.find("\r\n") << std::endl;
	std::cout << "s3 : " << s3.find("\r\n") << std::endl;
	return 0;
}