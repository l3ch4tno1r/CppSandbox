#include <iostream>
#include <sstream>

#define SEPARATOR(X) std::cout << "---------- " << X << " ----------" << std::endl

int main()
{
	SEPARATOR(1);
	std::stringstream sstr;

	SEPARATOR(2);
	sstr << 1 << 2;

	SEPARATOR(3);
	std::string str;

	sstr >> str;

	std::cout << str << std::endl;

	SEPARATOR(4);
	sstr << 3 << 4;

	sstr >> str;

	std::cout << str << std::endl;

	SEPARATOR("End");

	std::cin.get();
}