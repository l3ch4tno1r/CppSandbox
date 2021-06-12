#include <iostream>
#include <complex>

using namespace std::complex_literals;

int main()
{
	std::complex<int> pos;
	std::complex<int> dir{ 1, 0 };

	std::complex<int> left{ 0,  1 };
	std::complex<int> right{ 0, -1 };

	std::cout << dir << std::endl;
	std::cout << dir * left << std::endl;

	pos += dir;

	dir *= left;

	pos += dir;

	std::cout << pos << std::endl;

	std::cin.get();
}