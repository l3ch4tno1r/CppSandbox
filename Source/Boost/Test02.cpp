#include <iostream>

#include <boost\format.hpp>

int main()
{
	std::cout << boost::format("(x, y) = (%1%, %2%)") % 0 % 1 << '\n';

	std::cin.get();
}