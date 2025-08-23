#include <iostream>
#include <vector>
#include <sstream>

std::vector<std::string> split_cmd(std::string cmd)
{
	std::vector<std::string> vec;
	std::istringstream stm(cmd);
	std::string word;
	while(stm >> word)
	{
		vec.push_back(word);
		std::cout << word << std::endl;
		// word.clear();
	}
	return vec;
}


int main()
{
	std::vector <std::string> vec = split_cmd(std::string("0 00 000 \t 0000\t\n00000\v000000"));
	
	// std::cout << vec.size() << std::endl;
	// for (size_t i = 0; i < vec.size(); i++)
	// 	std::cout << vec[i] << std::endl;
	return 0;
}