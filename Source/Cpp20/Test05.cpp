#include <iostream>
#include <ranges>

int main()
{
	std::string str{ "one two three four five" };

	for (const auto& token : str | std::views::lazy_split(' '))
	{
		using test = decltype(token.front());
	}
		std::cout << token << '\n';

	std::cin.get();
}