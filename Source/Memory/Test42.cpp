#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

#include "_MemTracker.h"

int main()
{
	{
		std::vector<int> vec;

		vec.reserve(200);

		for (int i = 0; i < vec.capacity(); ++i)
			vec.emplace_back(i + 1);

		std::random_device rd;
		std::mt19937 g(rd());

		std::shuffle(vec.begin(), vec.end(), g);

		{
			auto session = MemTracker::Get().BeginScopeBasedSession();

			std::sort(vec.begin(), vec.end());
		}

		std::shuffle(vec.begin(), vec.end(), g);

		{
			auto session = MemTracker::Get().BeginScopeBasedSession();

			std::stable_sort(vec.begin(), vec.end());
		}
	}

	std::cin.get();
}