#include <iostream>

#include <LCN_Math/Source/Geometry/Vector.h>

int main()
{
	Vector3Df a = {  3, 1, 1 };
	Vector3Df b = { -4, 5, 1 };

	Vector3Df crossp = a ^ b;

	std::cout << crossp << std::endl;

	std::cin.get();
}