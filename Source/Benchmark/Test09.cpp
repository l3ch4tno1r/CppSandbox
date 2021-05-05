#include <iostream>
#include <vector>
#include <fstream>

#include <Utilities/Source/Benchmarking.h>

template<class ForwardIt, typename ValType>
ForwardIt LinearSearch(ForwardIt begin, ForwardIt end, const ValType& value)
{
	ForwardIt it = begin;

	while (it != end)
	{
		if (*it == value)
			break;

		++it;
	}

	return it;
}

template<class RandomIt, typename ValType>
RandomIt BinarySearchRecursive(RandomIt begin, RandomIt end, const ValType& value)
{
	if (begin == end)
		return begin;

	RandomIt middle = begin + (end - begin) / 2;

	if (*middle == value)
		return middle;

	if (*middle > value)
		end = middle;
	else
		begin = middle;

	return BinarySearchRecursive(begin, end, value);
}

template<class RandomIt, typename ValType>
RandomIt BinarySearchIterative(RandomIt begin, RandomIt end, const ValType& value)
{
	RandomIt lower = begin;
	RandomIt upper = end;

	while (lower < upper)
	{
		RandomIt middle = lower + (upper - lower) / 2;

		if (*middle == value)
			return middle;

		if (*middle <= value)
			lower = middle;
		else
			upper = middle;
	}

	return lower;
}

int main()
{
	std::ofstream file("TestBench.csv", std::ios::out);

	if (!file)
		return EXIT_FAILURE;

	for (size_t i = 0; i < 15; ++i)
	{
		size_t size = 1000 * (1 << i);

		file << size << ';';

		std::vector<int> v;

		v.reserve(size);

		for (size_t i = 0; i < v.capacity(); ++i)
			v.emplace_back(i);

		int target = 600 * (1 << i);

		LCN::Benchmark::Session session;

		{
			LCN::Benchmark::SessionTimer timer(session);

			auto it = LinearSearch(v.begin(), v.end(), target);
		}

		file << session.Duration(LCN::Benchmark::DurationUnit::Microseconds) << ';';

		{
			LCN::Benchmark::SessionTimer timer(session);

			auto it = BinarySearchRecursive(v.begin(), v.end(), target);
		}

		file << session.Duration(LCN::Benchmark::DurationUnit::Microseconds) << ';';

		{
			LCN::Benchmark::SessionTimer timer(session);

			auto it = BinarySearchIterative(v.begin(), v.end(), target);
		}

		file << session.Duration(LCN::Benchmark::DurationUnit::Microseconds) << std::endl;
	}

	std::cout << "Done" << std::endl;

	std::cin.get();
}