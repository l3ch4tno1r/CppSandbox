#include <iostream>

#include "Matrix/StaticMatrix.h"
#include "Matrix/Vector3D.h"

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
	}
	catch (const std::exception& e)
	{	
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}