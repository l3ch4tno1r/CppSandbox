#include <iostream>
#include <vector>

#include "Utilities/Source/SplitString.h"
#include "Source/Memory/_MemTracker.h"

int main()
{
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		std::string str{ "Hello world ! My name is Matt Black !" };

		std::cout << "-------------------------------\n";

		auto splitResult = LCN::Split(str,  " ");

		for (auto view : splitResult)
			std::cout << view << '\n';
	}

	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		auto splitResult = LCN::Split("Hello gentlemen ! How you doin' ?", " ");

		for (auto view : splitResult)
			std::cout << view << '\n';
	}

	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		auto splitResult = LCN::Split("Now this is an interresting test !", " ");

		std::vector<std::string_view> vec;
		vec.reserve(splitResult.Count());

		std::copy(
			splitResult.begin(),
			splitResult.end(),
			std::back_inserter(vec));

		for (auto view : vec)
			std::cout << view << '\n';
	}

	std::cin.get();
}