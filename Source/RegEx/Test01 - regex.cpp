#include <iostream>
#include <string>
#include <sstream>
#include <regex>

#define SEPARATOR(X) std::cout << "------- " << X << " -------" << std::endl

int main()
{
	std::string str = "<Transformation>1.0 0. 0. 0. 0.0 1.0 0.0 5.0  0. 0. 1. 0. 0. 0. 0. 1.0</Transformation>";

	//std::regex  transformRegex("<Transformation>((-?\\d+\\.\\d* *){16})</Transformation>");
	std::regex  transformRegex("<(\\w+)>([\\w\\d(-?\\d+\\.\\d* *)]+)</\\1>");
	std::smatch matches;

	std::regex_search(str, matches, transformRegex);

	std::cout << matches.size() << std::endl;

	for (auto& m : matches)
		std::cout << m.str() << std::endl;

	/*
	std::stringstream sstr(matches[1].str());

	float tab[16];

	for (size_t i = 0; i < 16; ++i)
		sstr >> tab[i];

	for (size_t i = 0; i < 4; ++i)
	{
		for (size_t j = 0; j < 4; ++j)
			std::cout << tab[4 * i + j] << '\t';

		std::cout << std::endl;
	}
	*/

	std::cin.get();
}