#include <iostream>

unsigned long stringlen(const char* str)
{
	const char* end = str;

	while (*++end);

	return end - str;
}

int main()
{
	std::cout << stringlen("Hello") << '\n';
	std::cout << stringlen("")      << '\n';
	std::cout << stringlen("H")     << '\n';
	std::cout << stringlen("Yo")    << '\n';

	std::cin.get();
}