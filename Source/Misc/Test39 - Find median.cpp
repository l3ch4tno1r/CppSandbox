#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <random>

#include "Utilities/Source/Benchmarking.h"
#include "Utilities/Source/ErrorHandling.h"

template<typename ForwardIt>
void PrintSequence(ForwardIt begin, ForwardIt end)
{
	while (begin < end)
		std::cout << *begin++ << ' ';

	std::cout << std::endl;
}

#define SEPARATOR(X) std::cout << "-------- " << X << " --------" << std::endl;

// Partitions the set between elements that statisfies F and those which don't
// Returns the position of the first element that do not statisfy F
template<typename ForwardIt, typename UnaryPredicate>
ForwardIt Partition(ForwardIt begin, ForwardIt end, UnaryPredicate pred)
{
	ForwardIt last = begin;

	while (pred(*last))
		if (++last == end)
			return last;	

	ForwardIt it = last;

	while (++it != end)
	{
		if (!pred(*it))
			continue;

		auto val = *it;

		*it   = *last;
		*last = val;

		last++;
	}

	return last;
}

template<typename RandomIt, typename BinaryPredicate>
auto Select(size_t pos, RandomIt begin, RandomIt end, BinaryPredicate pred)
{
	const RandomIt target = begin + pos;

	RandomIt lower = begin;
	RandomIt upper = end;

	while (lower < upper)
	{
		RandomIt last = upper - 1;

		if (lower == last)
			return *lower;

		auto pivot = *last;

		RandomIt it = Partition(lower, last, [&](auto x)
		{
			return pred(x, pivot);
		});

		auto val = *it;
		*it = *last;
		*last = val;

		if (it == target)
			return *it;

		if (it < target)
			lower = it;
		else
			upper = it;
	}

	return *lower;
}

template<typename RandomIt>
auto Median(RandomIt begin, RandomIt end)
{
	return Select((size_t)(end - begin) / 2, begin, end, [](auto a, auto b) { return a <= b; });
}

int main()
{
	SEPARATOR(1);
	{
		std::array<int, 10> arr = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 };

		auto it = Partition(arr.begin(), arr.end(), [](int a)
			{
				return a % 2 == 0;
			});

		std::cout << (it == arr.end()) << std::endl;
	}

	SEPARATOR(2);
	{
		std::vector<int> vec;

		vec.reserve(15);

		for (int i = 0; i < 15; ++i)
			vec.emplace_back(i);
	
		//std::random_device rd;
		//std::mt19937 g(rd());

		std::shuffle(vec.begin(), vec.end(), std::default_random_engine());

		for (int i : vec)
			std::cout << i << ' ';

		std::cout << std::endl;

		auto it = Partition(vec.begin(), vec.end(), [](int a)
			{
				return a % 2 == 0;
			});

		for (int i : vec)
			std::cout << i << ' ';

		std::cout << std::endl;

		it = Partition(vec.begin(), vec.end(), [](int a)
			{
				return a % 2 == 0;
			});

		for (int i : vec)
			std::cout << i << ' ';

		std::cout << std::endl;
	}

	SEPARATOR(3);
	{
		std::vector<int> vec;

		vec.reserve(15);

		for (int i = 0; i < 15; ++i)
			vec.emplace_back(i);

		//std::random_device rd;
		//std::mt19937 g(rd());

		std::shuffle(vec.begin(), vec.end(), std::default_random_engine());

		int median = Select(9, vec.begin(), vec.end(), [](int a, int b)
		{
			return a <= b;
		});

		PrintSequence(vec.begin(), vec.end());

		std::cout << median << std::endl;
	}

	SEPARATOR(4);
	{
		std::vector<int> vec;

		vec.reserve(17);

		for (int i = 0; i < 17; ++i)
			vec.emplace_back(i);

		std::shuffle(vec.begin(), vec.end(), std::default_random_engine());

		PrintSequence(vec.begin(), vec.end());

		std::cout << Median(vec.begin(), vec.end()) << std::endl;

		PrintSequence(vec.begin(), vec.end());
	}

	std::cin.get();
}