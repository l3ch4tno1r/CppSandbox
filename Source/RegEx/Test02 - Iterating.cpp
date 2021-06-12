#include <iostream>
#include <regex>
#include <sstream>

int main()
{
	const std::regex re("(\\d+)([a-zA-Z]+)");
	const std::string inputs[] = {
		"1a2b3c4d",
		"3chat4souris12chiens"
	};

	for (const auto& input : inputs)
	{
		std::sregex_iterator it(input.begin(), input.end(), re);
		std::sregex_iterator end;

		while (it != end)
		{
			const std::smatch& match = *it;
		
			std::stringstream sstr(match[1]);

			int i;
			sstr >> i;

			while (i-- > 0)
				std::cout << match[2];

			++it;
		}

		std::cout << std::endl;
	}

	std::cin.get();
}