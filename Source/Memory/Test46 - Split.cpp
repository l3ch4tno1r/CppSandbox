#include <iostream>

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

	std::cin.get();
}