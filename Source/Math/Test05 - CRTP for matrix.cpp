#include <iostream>

#include "Matrix/Matrix.h"

int main()
{
	try
	{
		Matrix<float, 2, 2> mat = {
			1.0f, 2.0f,
			3.0f, 4.0f
		};

		auto imat = mat.Invert();

		std::cout << imat << std::endl;
		std::cout << mat * imat << std::endl;
	}
	catch (const std::exception& e)
	{	
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}