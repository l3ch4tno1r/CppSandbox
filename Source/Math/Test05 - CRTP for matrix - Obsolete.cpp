#include <iostream>

#include "LCN_Math/Source/_Matrix/StaticMatrix.h"
#include "LCN_Math/Source/_Geometry/3D/Vector3D.h"

#define SEPARATOR(X) std::cout << "-------- " << X << " --------" << std::endl

int main()
{
	try
	{
		StaticMatrix<float, 3, 3> mat = {
			0, 1, 0,
			1, 0, 0,
			0, 0, 1,
		};

		std::cout << mat.Trace() << std::endl;

		SEPARATOR(1);

		auto imat = mat.Invert();

		std::cout << imat << std::endl;

		SEPARATOR(2);

		Vector3D<float> vec1(1, 2, 3);
		Vector3D<float> vec2(4, 5, 6);
		
		std::cout << vec1 << std::endl;
		std::cout << mat * vec1 << std::endl;

		std::cout << vec1.Norm() << std::endl;

		Vector3D<float>::HVectorType hvec1 = vec1.Homogenous();

		std::cout << hvec1 << std::endl;

		SEPARATOR("Cross product");

		Vector3D<float> vec3 = vec1 + vec2;

		std::cout << vec3 << std::endl;

		vec3 = vec1 ^ vec2;

		std::cout << vec3 << std::endl;

		std::cout << 2.0f * vec3 << std::endl;
		std::cout << vec3 * -1.0f << std::endl;

		SEPARATOR(4);

		Vector3D<float> v = (mat * vec1) ^ vec2;

		std::cout << vec3.Line() << ", " << vec3.Column() << std::endl;

		SEPARATOR(5);
	}
	catch (const std::exception& e)
	{	
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}