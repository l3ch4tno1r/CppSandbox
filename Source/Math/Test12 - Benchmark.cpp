#include <iostream>

#include <Utilities/Source/Benchmarking.h>

#include <LCN_Math/Source/Matrix/MatrixBlock.h>
#include <LCN_Math/Source/Matrix/Transpose.h>
#include <LCN_Math/Source/Geometry/Transform.h>

using namespace LCN::Benchmark;

int main()
{
	const size_t iterations = 200000;

	LCN::Transform3Df T3D;
	LCN::Transform2Df T2D;

	std::future<Session> session3D1 = TimePerformanceAsync(iterations, [&]()
		{
			auto iT = T3D.Inverse();
		});

	std::future<Session> session2D1 = TimePerformanceAsync(iterations, [&]()
		{
			auto iT = T2D.Inverse();
		});

	std::future<Session> session3D2 = TimePerformanceAsync(iterations, [&]()
		{
			auto iT = T3D.QuickInverse();
		});

	std::future<Session> session2D2 = TimePerformanceAsync(iterations, [&]()
		{
			auto iT = T2D.QuickInverse();
		});

	long long _3Dd1 = session3D1.get().Duration(DurationUnit::Milliseconds);
	long long _2Dd1 = session2D1.get().Duration(DurationUnit::Milliseconds);
	long long _3Dd2 = session3D2.get().Duration(DurationUnit::Milliseconds);
	long long _2Dd2 = session2D2.get().Duration(DurationUnit::Milliseconds);

	std::cout << "Session duration 2D : " << _2Dd1 << "ms" << std::endl;
	std::cout << "Session duration 2D : " << _2Dd2 << "ms" << std::endl;

	std::cout << "Gain 2D: " << (double)_2Dd1 / (double)_2Dd2 << std::endl;

	std::cout << "Session duration 3D : " << _3Dd1 << "ms" << std::endl;
	std::cout << "Session duration 3D : " << _3Dd2 << "ms" << std::endl;

	std::cout << "Gain 3D: " << (double)_3Dd1 / (double)_3Dd2 << std::endl;

	std::cin.get();
}