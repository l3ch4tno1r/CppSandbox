#include <iostream>
#include <algorithm>
#include <array>

int main()
{
	std::array<int, 15> arr = { 91, 27, 18, 53, 46, 86, 79, 6, 17, 8, 73, 16, 56, 55, 14 };

	std::make_heap(arr.begin(), arr.end());

	for (int e : arr)
		std::cout << e << std::endl;

	std::pop_heap(arr.begin(), arr.end());

	std::cout << '\n';

	for (int e : arr)
		std::cout << e << std::endl;

	std::cin.get();
}