#include <iostream>
#include <optional>

#include "Utilities/Source/Noisy.h"

#define SEPARATOR(X) std::cout << "--------- " << X << " ---------\n";

struct Test
{
	Test(const Noisy& n) : noisy{ n } {}
	Test(Noisy&& n) noexcept : noisy{ std::move(n) } {}

	Noisy noisy;
};

int main()
{
	SEPARATOR(1)
	{
		auto otest1 = std::optional<Test>{ std::in_place, Noisy{} };
	}

	SEPARATOR(2)
	{
		Noisy n;

		auto otest2 = std::optional<Test>{ std::in_place, n };
	}

	std::cin.get();
}