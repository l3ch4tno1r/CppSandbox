#include <iostream>

struct Vec
{
	float x, y, z;
};

int main()
{
	std::cout << sizeof(Vec)   << std::endl;
	std::cout << sizeof(Vec*)  << std::endl;
	std::cout << sizeof(Vec&)  << std::endl;
	std::cout << sizeof(Vec&&) << std::endl;

	std::cin.get();
}