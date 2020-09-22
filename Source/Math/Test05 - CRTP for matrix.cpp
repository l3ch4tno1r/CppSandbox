#include <iostream>

#include "Matrix/Matrix.h"
#include "Matrix/MatrixOperations.h"

int main()
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

	std::cin.get();
}