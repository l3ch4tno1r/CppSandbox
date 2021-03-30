#include <iostream>

#include "External/entt/include/entt.hpp"

int main()
{
	entt::registry reg;

	entt::entity entities[] = {
		reg.create(),
		reg.create(),
		reg.create(),
		reg.create()
	};

	reg.emplace<int>(entities[0], 0);
	reg.emplace<char>(entities[0], 'a');

	reg.emplace<int>(entities[1], 1);
	reg.emplace<float>(entities[1], 1.0f);
	reg.emplace<char>(entities[1], 'b');

	reg.emplace<int>(entities[2], 2);
	reg.emplace<float>(entities[2], 2.0f);

	reg.emplace<int>(entities[3], 3);
	reg.emplace<char>(entities[3], 'c');

	reg.group<int, char>().each([](int a, char c)
		{
			std::cout << a << ", " << c << std::endl;
		});

	std::cout << std::endl;

	reg.group<int, char>(entt::exclude<float>).each([](int a, char c)
		{
			std::cout << a << ", " << c << std::endl;
		});

	std::cout << std::endl;

	reg.group<int, char>(entt::exclude<float, double>).each([](int a, char c)
		{
			std::cout << a << ", " << c << std::endl;
		});

	std::cin.get();
}