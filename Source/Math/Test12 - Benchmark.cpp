#include <iostream>

#include <Utilities/Source/Benchmarking.h>

#include <LCN_Math/Source/Matrix/MatrixBlock.h>
#include <LCN_Math/Source/Matrix/Transpose.h>
#include <LCN_Math/Source/Geometry/Transform.h>

using namespace LCN::Benchmark;

int main()
{
	const size_t iterations = 200000;

	LCN::Transform3Df T;

	std::future<Session> session1 = TimePerformanceAsync(iterations, [&]()
		{
			auto iT = T.Inverse();
		});

	std::future<Session> session2 = TimePerformanceAsync(iterations, [&]()
		{
			auto iT = T.QuickInverse();
		});

	long long d1 = session1.get().Duration(DurationUnit::Milliseconds);
	long long d2 = session2.get().Duration(DurationUnit::Milliseconds);

	std::cout << "Session duration : " << d1 << "ms" << std::endl;
	std::cout << "Session duration : " << d2 << "ms" << std::endl;

	std::cout << "Gain : " << (double)d1 / (double)d2 << std::endl;

	std::cin.get();
}