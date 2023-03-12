#include <iostream>

#include <boost/phoenix/phoenix.hpp>
#include <boost/format.hpp>

int main()
{	
	namespace ph = boost::phoenix::placeholders;
	ph::arg1_type x;
	ph::arg2_type y;
	ph::arg3_type z;

	auto expr = x + 2 * y + x * x;
	std::cout << expr(2, 1) << '\n';

	std::cout << boost::format("Hello world %1%") % 12 << std::endl;

	std::cin.get();
}