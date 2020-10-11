#include <iostream>

#include "Eigen/Core"

int main()
{
	std::cout << "Eigen version : " << EIGEN_MAJOR_VERSION << '.' << EIGEN_MINOR_VERSION << std::endl;

	Eigen::Matrix<float, 3, 3> mat;

	mat <<
		1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f,
		7.0f, 8.0f, 9.0f;

	std::cout << mat(0, 0) << std::endl;

	std::cin.get();
}