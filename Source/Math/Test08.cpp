#include <iostream>

#include <LCN_Math/Source/Matrix/StaticMatrix.h>

int main()
{
	StaticMatrix<float, 3, 3> mat = {
		1, 0, 0,
		0, 0, 1,
		0, 1, 0
	};

	StaticMatrix<float, 3, 1> vec1 = { 1, 2, 3 };
	StaticMatrix<float, 3, 1> vec2;

	auto expr = mat * vec1;

	std::cout << std::boolalpha << expr.ContainsRefTo(mat) << ' ' << expr.ContainsRefTo(vec1) << ' ' << expr.ContainsRefTo(vec2) << std::endl;

	vec1 = expr;
	vec2 = expr;

	std::cout << vec1 << std::endl;
	std::cout << vec2 << std::endl;


	std::cin.get();
}