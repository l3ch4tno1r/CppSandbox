#include <iostream>
#include <functional>

int main()
{
	std::cout << sizeof(std::function<void()>) << std::endl;
	std::cout << sizeof(std::function<void(int)>) << std::endl;

	std::cin.get();
}