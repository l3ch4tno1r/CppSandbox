#include <iostream>

#include "LCN_Math/Source/Matrix/Matrix.h"
#include "LCN_Math/Source/Matrix/MatrixBlock.h"

int main()
{
	LCN::Matrix4f m = {
		1, 0,  0, 1,
		0, 0, -1, 2,
		0, 1,  0, 3,
		0, 0,  0, 1
	};

	LCN::MatrixBlock<LCN::Matrix4f, 0, 0, 3, 3> r(m);
	LCN::MatrixBlock<LCN::Matrix4f, 0, 3, 3, 1> t(m);

	std::cout << m << std::endl;
	std::cout << r << std::endl;
	std::cout << t << std::endl;

	t(1, 0) = 7;

	std::cout << m << std::endl;

	//LCN::MatrixBlock<LCN::Matrix4f, 0, 0, 5, 1> test1(m);
	//LCN::MatrixBlock<LCN::Matrix4f, 0, 0, 1, 5> test2(m);
	//LCN::MatrixBlock<LCN::Matrix4f, 4, 0, 3, 3> test3(m);
	//LCN::MatrixBlock<LCN::Matrix4f, 0, 4, 3, 3> test4(m);
	//LCN::MatrixBlock<LCN::Matrix4f, 0, 3, 3, 2> test5(m);
	//LCN::MatrixBlock<LCN::Matrix4f, 3, 0, 2, 3> test6(m);

	std::cin.get();
}