#include <iostream>
#include <vector>
#include <sstream>


class A
{
public:
	int x = 0;
	A(){};
	A(int i){x = i;};
	~A(){};
};

int main()
{
	std::vector <A> aaa;
	std::vector <A*> ccc;

	aaa.push_back(A(1));
	aaa.push_back(A(2));
	aaa.push_back(A(3));
	ccc.push_back(&aaa[0]);
	ccc.push_back(&aaa[1]);
	ccc.push_back(&aaa[2]);

	std::cout << ccc[0]->x << std::endl;
	std::cout << ccc[1]->x << std::endl;
	std::cout << ccc[2]->x << std::endl;

	aaa.erase(aaa.begin() + 0);
	std::cout << ccc[0]->x << std::endl;
	std::cout << ccc[1]->x << std::endl;
	std::cout << ccc[2]->x << std::endl;
	return 0;
}