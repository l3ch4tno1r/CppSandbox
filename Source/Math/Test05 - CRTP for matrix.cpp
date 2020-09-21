#include <iostream>

#include "Matrix/Matrix.h"
#include "Matrix/MatrixOperations.h"

int main()
{
	Matrix<float, 2, 3> mat1 = {
		1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0
	};

	Matrix<float, 3, 1> vec1 = {
		1.0f,
		1.0f,
		1.0f
	};

	std::cout << mat1 * vec1 << std::endl;

	std::cin.get();
}