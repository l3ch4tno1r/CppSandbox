#include <iostream>

#include <LCN_Math/Source/Matrix/Matrix.h>
#include <LCN_Math/Source/Matrix/MatrixN.h>

#define SEPARATOR(X) std::cout << "-------- " << X << " --------" << std::endl

using Matrix3f = Matrix<float, 3, 3>;

int main()
{
	SEPARATOR(1);
	{
		Matrix3f mat = {
			1, 0, 0,
			0, 0, 1,
			0, 1, 0
		};

		Matrix3f imat = mat.Invert();

		std::cout << imat << std::endl;

		std::cout << mat * imat << std::endl;

		Matrix3f test = mat;

		std::cout << test << std::endl;

		test = mat * imat;

		std::cout << test << std::endl;

		test = Matrix3f({
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		});

		std::cout << test << std::endl;

		Matrix3f test2 = test * mat;

		std::cout << test2 << std::endl;

		test2 = {
			1, 1, 1,
			2, 2, 2,
			3, 3, 3
		};

		std::cout << test2 << std::endl;
	}

	SEPARATOR(2);
	{
		MatrixN<float> dmat(3, 3);

		/*
		dmat = {
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		};
		*/

		Matrix3f smat = {
			1, 0, 0,
			0, 0, 1,
			0, 1, 0
		};

		auto prod = smat * dmat;

		std::cout << prod << std::endl;
	}

	std::cin.get();
}