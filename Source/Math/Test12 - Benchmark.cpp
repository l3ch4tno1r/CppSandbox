#include <iostream>

#include <Utilities/Source/Benchmarking.h>

#include <LCN_Math/Source/Matrix/MatrixBlock.h>
#include <LCN_Math/Source/Matrix/Transpose.h>
#include <LCN_Math/Source/Geometry/Transform.h>

int main()
{
	const size_t iterations = 100000;

	LCN::Transform3Df T;

	Benchmark::TimePerformance(iterations, [&]()
		{
			auto iT = T.Invert();
		});

	Benchmark::TimePerformance(iterations, [&]()
		{
			auto iT = T.QuickInverse();
		});

	std::cin.get();
}