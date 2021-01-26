#include <iostream>
#include <tuple>
#include <string>

int main()
{
	std::tuple<int, std::string> t = std::make_tuple<int, std::string>(2, "Hello world !");

	std::cout << std::get<1>(t) << std::endl;
	std::cout << std::get<1>(t) << std::endl;

	std::cin.get();
}