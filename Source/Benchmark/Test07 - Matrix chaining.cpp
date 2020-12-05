#include <iostream>
#include <functional>
#include <future>

#include <LCN_Math/Source/Matrix/MatrixN.h>
#include <Utilities/Source/Benchmarking.h>

int main()
{
#ifdef DEBUG
	const size_t iterations = 1000;
#else
	const size_t iterations = 20000;
#endif // DEBUG

	{
		MatrixN<float> A(10, 30), B(30, 5), C(5, 60);
		MatrixN<float> result1(10, 60), result2(10, 60);

		Timer timer;

		std::launch policy = std::launch::async;

		std::future<void> f1 = Benchmark::TimePerformanceAsync(iterations, [&]()
		{
			result1 = (A * B) * C;
		});

		std::future<void> f2 = Benchmark::TimePerformanceAsync(iterations, [&]()
		{
			result2 = A * (B * C);
		});

		f1.get();
		f2.get();
	}

	std::cin.get();
}