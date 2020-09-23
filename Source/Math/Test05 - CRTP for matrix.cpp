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

		Matrix<float, 2, 1> vec = {
			 1.0f,
			-1.0f
		};

		std::cout << mat * vec << std::endl;

		mat.SwapLines(0, 1);

		std::cout << mat << std::endl;

		mat.ScaleLine(1, 2.0f);

		std::cout << mat << std::endl;
		std::cout << mat.Trace() << std::endl;

		mat.CombineLines(1, 1.0f, 0, -1.0f);

		std::cout << mat << std::endl;

		std::cout << mat.Det() << std::endl;
		std::cout << mat.GaussElimination() << std::endl;
		std::cout << mat << std::endl;
	}
	catch (const std::exception& e)
	{	
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}