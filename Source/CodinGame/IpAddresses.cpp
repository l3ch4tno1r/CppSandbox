#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>

struct IPV4
{
	uint32_t Data{ 0 };
};

std::istream& operator>>(std::istream& is, IPV4& ipv4)
{
	uint32_t shift = 24;
	char c;

	do
	{
		uint32_t temp;
		is >> temp >> c;
		ipv4.Data |= (temp << shift);

		shift -= 8;
	}
	while (c == '.' && shift >= 0);

	return is;
}

int main()
{
	std::ifstream file("Ressources/CodinGame/IP Addresses - 01.txt", std::ios::in);

	if (!file)
		throw std::exception("File not found !");

	int nLine;
	file >> nLine;

	for (; nLine > 0; nLine--)
	{
		IPV4 subNet, address;
		uint32_t mask;

		file >> subNet >> mask >> address;

		uint32_t test = std::numeric_limits<uint32_t>::max();

		test >>= 32 - mask;
		test <<= 32 - mask;
	}

	std::cin.get();
}