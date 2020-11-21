#include <iostream>

#include <Utilities/Source/Benchmarking.h>

#define SEPARATOR(X) std::cout << "-------- " << X << " --------" << std::endl

class Mat
{
public:
	float operator()(int i, int j) const { return m_Data[i][j]; }

private:
	float m_Data[3][3];
};

int main()
{
	const int iterations = 10000000;

	SEPARATOR("Accessing single variable");
	{
		float a = 12.0f;

		Benchmark::TimePerformance(iterations, [&]()
		{
			float b = a;
		});
	}

	SEPARATOR("Accessing single array element");
	{
		float tab[] = { 1, 2, 3 };
		int i = 2;

		Benchmark::TimePerformance(iterations, [&]()
		{
			float e = tab[i];
		});
	}

	SEPARATOR("Accessing double array element");
	{
		float mat[3][3] = {
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		};

		int i = 1, j = 2;

		Benchmark::TimePerformance(iterations, [&]()
		{
			float e = mat[i][j];
		});

		Mat mat2;

		Benchmark::TimePerformance(iterations, [&]()
		{
			float e = mat2(i,j);
		});
	}

	std::cin.get();
}