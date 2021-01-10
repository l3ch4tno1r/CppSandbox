#include <iostream>

#include "Eigen/Core"
#include "Eigen/Geometry"

#include <LCN_Math/Source/Utilities/Angles.h>

#define SEPARATOR(X) std::cout << "---------- " << X << " ----------" << std::endl

template<class EL, class ER>
inline typename Eigen::MatrixBase<EL>::PlainObject
operator^(
	const Eigen::MatrixBase<EL>& el,
	const Eigen::MatrixBase<ER>& er)
{
	return el.cross3(er);
}

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

	SEPARATOR(3);
	{
		Eigen::Vector4f vec1 = { 1, 2, 3, 0 };
		Eigen::Vector4f vec2 = { 0, 0, 1, 1 };

		auto vec3 = vec1.cross3(vec2);

		std::cout << vec3 << std::endl;

		auto vec4 = vec1 ^ vec2;

		std::cout << vec4 << std::endl;
	}

	SEPARATOR(4);
	{
		Eigen::Affine3f transform;

		transform.setIdentity();

		transform.rotate(Eigen::AngleAxisf(TORAD(90.0f), Eigen::Vector3f::UnitZ()));

		//std::cout << transform.rotation() << std::endl;
		//std::cout << transform.translation() << std::endl;

		Eigen::Affine3f::TranslationPart translation = transform.translation();

		std::cout << "Size of rotation : " << sizeof(translation) << std::endl;

		translation << 1, 4, 3;
		transform.linear().setIdentity();

		translation[0] = 2;

		std::cout << transform.matrix() << std::endl;
	}

	std::cin.get();
}