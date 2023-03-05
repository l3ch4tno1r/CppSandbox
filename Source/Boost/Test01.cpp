#include <iostream>
#include <string>

#include <boost\spirit\include\karma.hpp>

#include "Source\Memory\_MemTracker.h"

int main()
{
	using boost::spirit::karma::int_;
	using boost::spirit::karma::float_;
	using boost::spirit::karma::generate;

	auto session = MemTracker::Get().BeginScopeBasedSession();

	std::vector<int> vec{ 1, 2, 5, 4, 3, 0, 4, -1, 6 };
	std::string result;

	auto expr = int_ << float_;

	bool generationOK = generate(
		std::back_inserter(result),
		"{ " << int_ << *(" | " << int_) << " }",
		vec);

	std::cout << (generationOK ? result : "Kaputt !!!") << '\n';

	std::cin.get();
}