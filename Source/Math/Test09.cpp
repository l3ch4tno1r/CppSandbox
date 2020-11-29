#include <iostream>

#include <LCN_Math/Source/Matrix/Matrix.h>
#include <LCN_Math/Source/Matrix/MatrixN.h>

#include "MiscellaneousTests/Source/Memory/_MemTracker.h"

#define SEPARATOR(X) std::cout << "-------- " << X << " --------" << std::endl

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
		MemTracker::ScopeBasedSession session = MemTracker::Get().BeginScopeBasedSession();

		MatrixN<float> dmat(3, 4, {
			1, 1, 1, 1,
			2, 2, 2, 2,
			3, 3, 3, 3
		});

		std::cout << dmat << std::endl;

		Matrix3f smat = {
			1, 0, 0,
			0, 0, 1,
			0, 1, 0
		};

		auto prod = smat * dmat;

		std::cout << prod << std::endl;

		MatrixN<float> dmat2 = std::move(dmat);

		std::cout << dmat2 << std::endl;

		dmat = std::move(dmat2);

		std::cout << dmat << std::endl;

		dmat2 = dmat;

		std::cout << dmat2 << std::endl;

		dmat2 = prod;

		std::cout << dmat2 << std::endl;
	}

	SEPARATOR("Memory management");
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		MatrixN<char> dmat;

		dmat.Resize(4, 4);

		Matrix<char, 3, 3> smat = {
			'a', 'b', 'c',
			'd', 'e', 'f',
			'g', 'h', 'i',
		};

		dmat = smat;

		std::cout << dmat << std::endl;
		std::cout << "dmat capacity    : " << dmat.Capacity() << std::endl;
		std::cout << "dmat actual size : " << dmat.Line() * dmat.Column() << std::endl;

		dmat.ShrinkToFit();

		std::cout << '\n';
		std::cout << "dmat capacity    : " << dmat.Capacity() << std::endl;
		std::cout << "dmat actual size : " << dmat.Line() * dmat.Column() << std::endl;

		MatrixN<char> dmat2 = std::move(dmat);

		std::cout << dmat2 << std::endl;
	}

	SEPARATOR("Done !");

	std::cin.get();
}