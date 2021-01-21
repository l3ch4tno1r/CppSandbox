#include <iostream>
#include <iomanip>

#include <LCN_Math/Source/Matrix/Matrix.h>
#include <LCN_Math/Source/Matrix/MatrixBlock.h>
#include <LCN_Math/Source/Geometry/Vector.h>

#define SEPARATOR(X) std::cout << "-------- " << X << " --------" << std::endl;

int main()
{
	SEPARATOR(1)
	{
		LCN::Matrix4f m = {
			1, 0,  0, 1,
			0, 0, -1, 2,
			0, 1,  0, 3,
			0, 0,  0, 1
		};

		LCN::Matrix<float, 4, 1> v = { 1, 1, 1 ,1 };

		const auto& ref = m;

		LCN::MatrixBlock<LCN::Matrix4f, 3, 3> r(m, 0, 0);
		LCN::MatrixBlock<LCN::Matrix4f, 3, 1> t(m, 0, 3);

		std::cout << m << std::endl;
		std::cout << r << std::endl;
		std::cout << t << std::endl;

		t(1, 0) = 7;

		std::cout << m << std::endl;

		std::cout << m.Columns()[2] << std::endl;
		std::cout << m.Rows()[1] << std::endl;

		std::cout << m * v << std::endl;

		//LCN::MatrixBlock<LCN::Matrix4f, 0, 0, 5, 1> test1(m);
		//LCN::MatrixBlock<LCN::Matrix4f, 0, 0, 1, 5> test2(m);
		//LCN::MatrixBlock<LCN::Matrix4f, 4, 0, 3, 3> test3(m);
		//LCN::MatrixBlock<LCN::Matrix4f, 0, 4, 3, 3> test4(m);
		//LCN::MatrixBlock<LCN::Matrix4f, 0, 3, 3, 2> test5(m);
		//LCN::MatrixBlock<LCN::Matrix4f, 3, 0, 2, 3> test6(m);
	}

	SEPARATOR(2)
	{
		LCN::HVector3Df hv1 = { 1, 2, -1, 0 };
		LCN::HVector3Df hv2 = { 3, 4, -1, 0 };

		std::cout << (hv1.Vector() ^ hv2.Vector()) << std::endl;
	}

	std::cin.get();
}