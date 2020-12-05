#include <iostream>
#include <functional>
#include <future>
#include <array>

#include <LCN_Math/Source/Matrix/Matrix.h>
#include <LCN_Math/Source/Matrix/MatrixN.h>

#include <Utilities/Source/Benchmarking.h>

#define SEPARATOR(X) std::cout << "-------- " << X << " --------" << std::endl

struct MatrixDimension
{
	size_t L, C;

	template<class Derived>
	MatrixDimension(const MatrixBase<Derived>& mat) :
		L(mat.Line()),
		C(mat.Column())
	{}
};

template<class RandomAccessItertor>
size_t MatrixProductCost(RandomAccessItertor begin, RandomAccessItertor end)
{
	auto size = end - begin;

	if (size < 2)
		return 0;

	for (RandomAccessItertor it = begin; it != end - 1; ++it)
		if (it->C != (it + 1)->L)
			throw std::exception("Product is undefined.");

	size_t cost = std::numeric_limits<size_t>::max();

	for (RandomAccessItertor it = begin; it != end - 1; ++it)
	{
		size_t costl = MatrixProductCost(begin, it + 1);
		size_t costr = MatrixProductCost(it + 1, end);

		size_t costg = begin->L * it->C * (end - 1)->C;

		cost = std::min(cost, costl + costr + costg);
	}

	return cost;
}

int main()
{
#ifdef DEBUG
	const size_t iterations = 10000;
#else
	const size_t iterations = 200000;
#endif // DEBUG

	SEPARATOR("Test00");
	{
		Matrix<float, 2, 5> A;
		Matrix<float, 5, 3> B;
		Matrix<float, 3, 4> C;
		Matrix<float, 4, 1> D;

		MatrixDimension dimensions[] = {
			MatrixDimension(A),
			MatrixDimension(B),
			MatrixDimension(C),
			MatrixDimension(D)
		};

		std::cout << "Product cost AB   : " << MatrixProductCost(&dimensions[0], &dimensions[2]) << std::endl;
		std::cout << "Product cost BC   : " << MatrixProductCost(&dimensions[1], &dimensions[3]) << std::endl;
		std::cout << "Product cost CD   : " << MatrixProductCost(&dimensions[2], &dimensions[4]) << std::endl;

		std::cout << "Product cost ABC  : " << MatrixProductCost(&dimensions[0], &dimensions[3]) << std::endl;
		std::cout << "Product cost BCD  : " << MatrixProductCost(&dimensions[1], &dimensions[4]) << std::endl;

		std::cout << "Product cost ABCD : " << MatrixProductCost(&dimensions[0], &dimensions[4]) << std::endl;
	}

	SEPARATOR("Test01");
	{
		Matrix<float, 2, 5> A;
		Matrix<float, 5, 3> B;
		Matrix<float, 3, 4> C;
		Matrix<float, 4, 1> D;
		Matrix<float, 2, 1> R;

		Benchmark::TimePerformance(iterations, [&]()
		{
			R = ((A * B) * C) * D;
		});

		Benchmark::TimePerformance(iterations, [&]()
		{
			R = (A * (B * C)) * D;
		});

		Benchmark::TimePerformance(iterations, [&]()
		{
			R = (A * B) * (C * D);
		});

		Benchmark::TimePerformance(iterations, [&]()
		{
			R = A * ((B * C) * D);
		});

		Benchmark::TimePerformance(iterations, [&]()
		{
			R = A * (B * (C * D));
		});
	}

	SEPARATOR("Test01_5");
	{
		MatrixN<float> A(10, 30), B(30, 5), C(5, 60);

		MatrixDimension dimensions[] = {
			MatrixDimension(A),
			MatrixDimension(B),
			MatrixDimension(C)
		};

		std::cout << "Product cost AB   : " << MatrixProductCost(&dimensions[0], &dimensions[2]) << std::endl;
		std::cout << "Product cost BC   : " << MatrixProductCost(&dimensions[1], &dimensions[3]) << std::endl;

		std::cout << "Product cost ABC  : " << MatrixProductCost(&dimensions[0], &dimensions[3]) << std::endl;
	}

	/*
	SEPARATOR("Test02");
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
	*/

	std::cin.get();
}