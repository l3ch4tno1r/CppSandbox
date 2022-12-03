#include <iostream>
#include <fstream>
#include <ranges>
#include <string>
#include <sstream>
#include <charconv>
#include <optional>
#include <vector>

#include <Source/Memory/_MemTracker.h>
#include <Utilities/Source/SplitString.h>

uint32_t to_int_throw(std::string_view view)
{
	uint32_t result;

	auto convResult = std::from_chars(
		view.data(),
		view.data() + view.size(),
		result);

	if (convResult.ec != std::errc{ 0 })
		throw std::exception("wrong format !");

	return result;
}

std::optional<uint32_t> to_int_no_throw(std::string_view view)
{
	uint32_t result;

	auto convResult = std::from_chars(
		view.data(),
		view.data() + view.size(),
		result);

	if (convResult.ec != std::errc{ 0 })
		return std::nullopt;

	return result;
}

struct Line
{
	std::string Data;
};

std::istream& operator>>(std::istream& is, Line& line)
{
	std::getline(is, line.Data);

	return is;
}

enum class GenderEnum : char
{
	Female = 'F',
	Male   = 'M'
};

struct Record
{
	std::string Name;
	std::string Parent;
	uint32_t Birth;
	std::optional<uint32_t> Death;
	GenderEnum Gender;
};

int main()
{
	try
	{
		//auto session = MemTracker::Get().BeginScopeBasedSession();

		//std::ifstream istream{ "Ressources/CodinGame/Order of succession - A Simple Tree.txt", std::ios::in };
		std::ifstream istream{ "Ressources/CodinGame/Order of succession - Non Anglical People.txt", std::ios::in };

		if (!istream)
			throw std::exception("File not found !");

		std::vector<std::string_view> buff;

		auto istreamView = std::ranges::istream_view<Line>(istream)
			| std::views::drop(1)
			| std::views::transform([&](const Line& line) -> Record
			{
				buff.clear();

				auto splitResult = LCN::Split(line.Data, " ");

				std::copy(
					splitResult.begin(),
					splitResult.end(),
					std::back_inserter(buff));

				if (6 != buff.size())
					throw std::exception("Format error !");

				Record result;

				result.Name   = buff[0];
				result.Parent = buff[1];
				result.Birth  = to_int_throw(buff[2]);
				result.Death  = to_int_no_throw(buff[3]);
				result.Gender = GenderEnum{ buff[5][0] };

				return result;
			})
			| std::views::filter([](const Record& record) { return record.Gender == GenderEnum::Female; });

		for (const auto& l : istreamView)
		{
			std::cout << "--> " << l.Name << ", " << (char)l.Gender << " - " << l.Birth;

			if (l.Death)
				std::cout << " to " << *l.Death;

			std::cout << '\n';
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}