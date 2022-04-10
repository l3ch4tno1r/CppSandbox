#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <iterator>
#include <string_view>
#include <vector>
#include <algorithm>

const std::string_view morse[] = {
	{ ".-" },   /*A*/ { "-..." }, /*B*/ { "-.-." }, /*C*/ { "-.." },  /*D*/
	{ "." },    /*E*/ { "..-." }, /*F*/ { "--." },  /*G*/ { "...." }, /*H*/
	{ ".." },   /*I*/ { ".---" }, /*J*/ { "-.-" },  /*K*/ { ".-.." }, /*L*/
	{ "--" },   /*M*/ { "-." },   /*N*/ { "---" },  /*O*/ { ".--." }, /*P*/
	{ "--.-" }, /*Q*/ { ".-." },  /*R*/ { "..." },  /*S*/ { "-" },    /*T*/
	{ "..-" },  /*U*/ { "...-" }, /*V*/ { ".--" },  /*W*/ { "-..-" }, /*X*/
	{ "-.--" }, /*Y*/ { "--.." }, /*Z*/
};

template<typename ForwardIt>
ForwardIt RecursivePartition()
{}

int main()
{
	try
	{
		//std::ifstream input{ "Ressources/CodinGame/Resistance - Simple message.txt", std::ios::in };
		//std::ifstream input{ "Ressources/CodinGame/Resistance - Same code different word.txt", std::ios::in };
		std::ifstream input{ "Ressources/CodinGame/Resistance - Long sequence big dictionnary.txt", std::ios::in };

		if (!input)
			throw std::exception{ "File not found" };

		std::string msg;
		input >> msg;

		uint32_t count;
		input >> count;

		std::vector<std::string> dictionary;
		std::istream_iterator<std::string> it{ input }, end{};

		for (; it != end; ++it)
		{
			std::string translated;

			for (char c : *it)
				translated += morse[c - 'A'];

			dictionary.emplace_back(std::move(translated));
		}

		std::partition(
			dictionary.begin(),
			dictionary.end(),
			[](const std::string& str)
			{
				return str[0] == '.';
			});

		for (const auto& str : dictionary)
			std::cerr << str << '\n';
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << '\n';
	}

	std::cin.get();
}