#include <iostream>
#include <ranges>
#include <vector>
#include <concepts>

int main()
{
	std::vector<int> vec{ 1, 2, 3, 4, 5, 6 };

	auto isEven = [](int a)
	{
		return a % 2 == 0;
	};

	auto transform = [](int a)
	{
		struct
		{
			int i, j;
		}
		result{ a % 5, a / 5 };

		return result;
	};

	auto view = vec
		| std::views::filter(isEven)
		| std::views::transform(transform);

	for (auto a : view)
		std::cout << a.i << ", " << a.j << std::endl;

	using Test = decltype(std::views::filter(isEven));

	std::cin.get();
}