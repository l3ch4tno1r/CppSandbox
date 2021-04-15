#include <iostream>

enum class Test
{
	Zero,
	Un,
	Deux,
	Trois
};

int main()
{
	Test test = static_cast<Test>(8.3f);

	std::cin.get();
}