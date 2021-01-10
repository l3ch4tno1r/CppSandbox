#include <iostream>
#include <bitset>

#define SETBIT(X, bit)   X |=  (1 << bit)
#define RESETBIT(X, bit) X &= ~(1 << bit)

void Print(const std::bitset<8 * sizeof(float)>& set)
{
	std::cout << set[31] << ' ';

	for (size_t i = 30; i >= 23; --i)
		std::cout << set[i];

	std::cout << ' ';

	for (int i = 22; i >= 0; --i)
		std::cout << set[i];

	std::cout << std::endl;
}

int main()
{
	float a = 12.0f;
	unsigned long _a = *(unsigned long*)&a;

	std::bitset<8 * sizeof(float)> set(_a);

	Print(set);

	set[31] = true;

	_a = set.to_ulong();

	a = *(float*)&_a;

	std::cin.get();
}