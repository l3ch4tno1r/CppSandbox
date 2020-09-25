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

		Vector3D<float> vec(1,	2,	3);

		std::cout << vec << std::endl;
		std::cout << mat * vec << std::endl;
	}
	catch (const std::exception& e)
	{	
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}