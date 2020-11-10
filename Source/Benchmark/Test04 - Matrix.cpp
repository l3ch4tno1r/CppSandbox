#include <iostream>

#include "Utilities/Source/Benchmarking.h"
#include "LCN_Math/Source/Geometry/Vector.h"
#include "Eigen/Geometry"

#define SEPARATOR(X) std::cout << "-------- " << X << " --------" << std::endl

class NaiveVector3Df
{
public:
	NaiveVector3Df(float x, float y, float z) :
		m_x(x),
		m_y(y),
		m_z(z)
	{}

	float x() const { return m_x; }
	float y() const { return m_y; }
	float z() const { return m_z; }

	float& x() { return m_x; }
	float& y() { return m_y; }
	float& z() { return m_z; }

private:
	float m_x;
	float m_y;
	float m_z;

	friend NaiveVector3Df operator+(const NaiveVector3Df& a, const NaiveVector3Df& b);
};

NaiveVector3Df operator+(const NaiveVector3Df& a, const NaiveVector3Df& b)
{
	return NaiveVector3Df(a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z);
}

std::ostream& operator<<(std::ostream& stream, const NaiveVector3Df& vec)
{
	stream << '(' << vec.x() << ", " << vec.y() << ", " << vec.z() << ')';

	return stream;
}

int main()
{
	const int iterations = 1000000;

	SEPARATOR("Naive version");
	{
		NaiveVector3Df a(1, 2, 3);
		NaiveVector3Df b(4, 5, 6);
		NaiveVector3Df c(7, 8, 9);

		std::cout << a + b + c << std::endl;

		TimePerformance(iterations, [&a, &b, &c]()
		{
			NaiveVector3Df sum = a + b + c;
		});
	}

	SEPARATOR("Expression version");
	{
		Vector3Df a = { 1, 2, 3 };
		Vector3Df b = { 4, 5, 6 };
		Vector3Df c = { 7, 8, 9 };

		TimePerformance(iterations, [&a, &b, &c]()
		{
			Vector3Df sum = a + b + c;
		});
	}

	SEPARATOR("Expression version 2");
	{
		Vector3Df a = { 1, 2, 3 };
		Vector3Df b = { 4, 5, 6 };
		Vector3Df c = { 7, 8, 9 };

		auto expr = a + b + c;

		TimePerformance(iterations, [&expr]()
		{
			Vector3Df sum = expr;
		});
	}

	SEPARATOR("Eigen");
	{
		Eigen::Vector3f a(1, 2, 3);
		Eigen::Vector3f b(4, 5, 6);
		Eigen::Vector3f c(7, 8, 9);

		TimePerformance(iterations, [&a, &b, &c]()
		{
			Eigen::Vector3f sum = a + b + c;
		});
	}

	SEPARATOR("XXXXXXXXXXXXXXXXXXXXXXXX");

	SEPARATOR("Evaluating one value - Naive version");
	{
		NaiveVector3Df a(1, 2, 3);
		NaiveVector3Df b(4, 5, 6);
		NaiveVector3Df c(7, 8, 9);

		TimePerformance(iterations, [&a, &b, &c]()
		{
			float sumx = (a + b + c).x();
		});
	}

	SEPARATOR("Evaluating one value - Expression version");
	{
		Vector3Df a = { 1, 2, 3 };
		Vector3Df b = { 4, 5, 6 };
		Vector3Df c = { 7, 8, 9 };

		TimePerformance(iterations, [&a, &b, &c]()
		{
			float sumx = (a + b + c)(0, 0);
		});
	}

	SEPARATOR("Evaluating one value - Expression version 2");
	{
		Vector3Df a = { 1, 2, 3 };
		Vector3Df b = { 4, 5, 6 };
		Vector3Df c = { 7, 8, 9 };

		auto expr = a + b + c;

		TimePerformance(iterations, [&expr]()
		{
			float sumx = expr(0, 0);
		});
	}

	SEPARATOR("Evaluating one value - Eigen");
	{
		Eigen::Vector3f a(1, 2, 3);
		Eigen::Vector3f b(4, 5, 6);
		Eigen::Vector3f c(7, 8, 9);

		TimePerformance(iterations, [&a, &b, &c]()
		{
			float sumx = (a + b + c)(0, 0);
		});
	}

	SEPARATOR("Evaluating one value - Eigen 2");
	{
		Eigen::Vector3f a(1, 2, 3);
		Eigen::Vector3f b(4, 5, 6);
		Eigen::Vector3f c(7, 8, 9);

		auto expr = a + b + c;

		TimePerformance(iterations, [&expr]()
		{
			float sumx = expr(0, 0);
		});
	}

	std::cout << "Done" << std::endl;

	std::cin.get();
}
