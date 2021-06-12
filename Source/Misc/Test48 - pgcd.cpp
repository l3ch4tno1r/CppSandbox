#include <iostream>

int PGCD(int a, int b)
{
	if (a == 0)
		return b;

	return PGCD(b % a, a);
}

int main()
{
	int h = PGCD(156, 88);

	std::cout << 156 / h << '/' << 88 / h << std::endl;

	std::cin.get();
}