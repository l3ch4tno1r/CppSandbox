#include <iostream>

#include "Matrix2/StaticMatrix.h"
#include "Matrix2/Vector3D.h"

struct HVec
{
	char x, y, z;
	const char s = 's';

	char operator[](size_t i) const
	{
		return ((char*)this)[i];
	}
};

std::ostream& operator<<(std::ostream& stream, const HVec& vec)
{
	for (size_t i = 0; i < 4; i++)
		stream << vec[i];

	return stream;
}

int main()
{
	try
	{
		StaticMatrix<float, 3, 3> mat = {
			0, 1, 0,
			1, 0, 0,
			0, 0, 1
		};

		std::cout << mat.Trace() << std::endl;

		auto imat = mat.Invert();

		std::cout << imat << std::endl;

		Vector3D<float> vec1(1,	2, 3);
		Vector3D<float> vec2(4, 5, 6);

		std::cout << vec1 << std::endl;
		std::cout << mat * vec1 << std::endl;

		std::cout << vec1.Norm() << std::endl;

		Vector3D<float> vec3 = vec1 + vec2;

		std::cout << vec3 << std::endl;

		HVec v1 = { 'x', 'y', 'z' };
		HVec v2 = { 'X', 'Y', 'Z' };

		std::cout << (int)&(v1.s) << std::endl;
		std::cout << (int)&(v2.s) << std::endl;
	}
	catch (const std::exception& e)
	{	
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}