#include <iostream>
#include <fstream>

#include "Matrix/Stack/SMatrix.h"

using namespace LCNMath::Matrix::StaticMatrix;

int main()
{
	try
	{
		std::ofstream output("Result.csv", std::ios::out);

		if (!output)
			throw std::exception("File not found");

		Matrix<float, 8, 18> mat = {
			 0,  0,  0, 0,  0, -1,  0,  0,  1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, -1,  1, 0,  0,  0,  0,  1,  1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
			 0,  0,  1, 0,  0,  0,  0,  0,  0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
			 0,  0,  0, 0, -1, -1,  0,  1,  1, 0, 0, 0, 1, 0, 0, 0, 0, 0,
			-1, -1, -1, 0,  0,  0,  1,  1,  1, 0, 0, 0, 0, 1, 0, 0, 0, 0,
			 0,  0,  0, 1,  1,  1, -1, -1, -1, 0, 0, 0, 0, 0, 1, 0, 0, 0,
			 1,  0,  1, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
			 0,  0,  0, 1,  0,  1,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 1, 0
		};

		mat.GaussElimination();

		for (size_t i = 0; i < mat.Lines(); ++i)
		{
			for (size_t j = 0; j < mat.Columns(); ++j)
				output << mat(i, j) << ';';
			
			output << '\n';
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}