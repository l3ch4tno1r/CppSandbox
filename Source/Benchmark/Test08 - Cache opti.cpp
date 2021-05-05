#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

#include <Utilities/Source/Benchmarking.h>

#define SEPARATOR(X) std::cout << "-------- " << X << " --------" << std::endl;

int main()
{
	const size_t iterations = 100000;

	std::vector<int> tab(100 * 150, int(0));
	std::list<int> list;

	for (size_t i = 0; i < 100 * 150; ++i)
		list.emplace_back(0);

	SEPARATOR("Range based for loop")
	{
		auto session = LCN::Benchmark::TimePerformance(iterations, [&]()
		{
			for(int& i : tab)
				i = 1;
		});

		std::cout << "Session #1 duration : " << session.Duration(LCN::Benchmark::DurationUnit::Milliseconds) << std::endl;
	}

	SEPARATOR("Double for loop")
	{
		auto session = LCN::Benchmark::TimePerformance(iterations, [&]()
		{
			for (size_t i = 0; i < 150; ++i)
				for (size_t j = 0; j < 100; ++j)
					tab[i + 150 * j] = 2;
		});

		std::cout << "Session #2 duration : " << session.Duration(LCN::Benchmark::DurationUnit::Milliseconds) << std::endl;
	}

	{
		auto session = LCN::Benchmark::TimePerformance(iterations, [&]()
		{
			for (size_t j = 0; j < 100; ++j)
				for (size_t i = 0; i < 150; ++i)
					tab[i + 150 * j] = 3;
		});

		std::cout << "Session #3 duration : " << session.Duration(LCN::Benchmark::DurationUnit::Milliseconds) << std::endl;
	}

	SEPARATOR("Simple for loop")
	{
		auto session = LCN::Benchmark::TimePerformance(iterations, [&]()
		{
			for (size_t i = 0; i < tab.size(); ++i)
				tab[i] = 41;
		});

		std::cout << "Session #4.1 duration : " << session.Duration(LCN::Benchmark::DurationUnit::Milliseconds) << std::endl;
	}

	{
		auto session = LCN::Benchmark::TimePerformance(iterations, [&]()
		{
			size_t size = tab.size();

			for (size_t i = 0; i < size; ++i)
				tab[i] = 42;
		});

		std::cout << "Session #4.2 duration : " << session.Duration(LCN::Benchmark::DurationUnit::Milliseconds) << std::endl;
	}

	{
		const size_t size = tab.size();

		auto session = LCN::Benchmark::TimePerformance(iterations, [&]()
		{
			for (size_t i = 0; i < size; ++i)
				tab[i] = 43;
		});

		std::cout << "Session #4.3 duration : " << session.Duration(LCN::Benchmark::DurationUnit::Milliseconds) << std::endl;
	}

	{
		auto session = LCN::Benchmark::TimePerformance(iterations, [&]()
		{
			for (size_t i = 0; i < 100 * 150; ++i)
				tab[i] = 44;
		});

		std::cout << "Session #4.4 duration : " << session.Duration(LCN::Benchmark::DurationUnit::Milliseconds) << std::endl;
	}

	SEPARATOR("Raw pointers");
	{
		auto session = LCN::Benchmark::TimePerformance(iterations, [&]()
		{
			int* ptr2 = tab.data();
			int* end  = tab.data() + tab.size();

			while (ptr2 < end)
				*ptr2++ = 51;
		});

		std::cout << "Session #5.1 duration : " << session.Duration(LCN::Benchmark::DurationUnit::Milliseconds) << std::endl;
	}

	{
		auto session = LCN::Benchmark::TimePerformance(iterations, [&]()
		{
			int* ptr2   = tab.data();
			size_t size = tab.size();

			for(size_t i = 0; i < size; ++i)
				ptr2[i] = 52;
		});

		std::cout << "Session #5.2 duration : " << session.Duration(LCN::Benchmark::DurationUnit::Milliseconds) << std::endl;
	}
	
	SEPARATOR("Iterators")
	{
		auto session = LCN::Benchmark::TimePerformance(iterations, [&]()
		{
			for (auto it = tab.begin(); it != tab.end(); ++it)
				*it = 6;
		});

		std::cout << "Session #6 duration : " << session.Duration(LCN::Benchmark::DurationUnit::Milliseconds) << std::endl;
	}

	{
		auto session = LCN::Benchmark::TimePerformance(iterations, [&]()
		{
			auto end = tab.end();

			for(auto it = tab.begin(); it != end; ++it)
				*it = 7;
		});

		std::cout << "Session #7 duration : " << session.Duration(LCN::Benchmark::DurationUnit::Milliseconds) << std::endl;
	}

	{
		auto session = LCN::Benchmark::TimePerformance(iterations, [&]()
		{
			std::for_each(tab.begin(), tab.end(), [](int& i) { i = 8; });
		});

		std::cout << "Session #8 duration : " << session.Duration(LCN::Benchmark::DurationUnit::Milliseconds) << std::endl;
	}

	SEPARATOR("List")
	{
		auto session = LCN::Benchmark::TimePerformance(iterations, [&]()
		{
			for(int& i : list)
				i = 9;
		});

		std::cout << "Session #9 duration : " << session.Duration(LCN::Benchmark::DurationUnit::Milliseconds) << std::endl;
	}

	{
		auto session = LCN::Benchmark::TimePerformance(iterations, [&]()
		{
			std::for_each(list.begin(), list.end(), [](int& i) {i = 10;});
		});

		std::cout << "Session #10 duration : " << session.Duration(LCN::Benchmark::DurationUnit::Milliseconds) << std::endl;
	}

	SEPARATOR("Backwards iteration")
	{
		auto session = LCN::Benchmark::TimePerformance(iterations, [&]()
		{
			std::for_each(tab.rbegin(), tab.rend(), [](int& i) {i = 11;});
		});

		std::cout << "Session #11 duration : " << session.Duration(LCN::Benchmark::DurationUnit::Milliseconds) << std::endl;
	}

	std::cin.get();
}