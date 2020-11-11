#include <iostream>

int main()
{
	int* ptr1 = new int(0);

	delete ptr1;

	int* tab1 = new int[10];

	delete[] tab1;

	std::cin.get();
}