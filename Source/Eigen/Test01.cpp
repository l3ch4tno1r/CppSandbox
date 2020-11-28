#include <iostream>

#include "Eigen/Core"
#include "Eigen/Geometry"

#define SEPARATOR(X) std::cout << "---------- " << X << " ----------" << std::endl

int main()
{
	SEPARATOR(0);
	{
		std::cout << "Eigen version : " << EIGEN_MAJOR_VERSION << '.' << EIGEN_MINOR_VERSION << std::endl;

		Eigen::Matrix<float, 3, 3> mat;

		mat <<
			1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f;

		std::cout << mat(0, 0) << std::endl;
	}

	SEPARATOR(1);
	{
		Eigen::Vector3f vec(1, 2, 3);

		vec.cross(vec);

		auto d = vec.dot(vec);

		vec.x() = 2;

		std::cout << vec.y() << std::endl << std::endl;
		auto vec4 = vec.homogeneous();
		std::cout << vec4 + vec4 << std::endl;
	}

	SEPARATOR(2);
	{
		Eigen::Matrix3f mat;

		auto tmat = mat.transpose().eval();
	}

	std::cin.get();
}