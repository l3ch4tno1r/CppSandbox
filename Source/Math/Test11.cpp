#include <iostream>
#include <iomanip>

#include <LCN_Math/Source/Matrix/Matrix.h>
#include <LCN_Math/Source/Matrix/MatrixBlock.h>
#include <LCN_Math/Source/Matrix/Transpose.h>

#include <LCN_Math/Source/Geometry/Vector.h>
#include <LCN_Math/Source/Geometry/Transform.h>

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

		std::cout << ref.Columns()[0] << std::endl;
		std::cout << ref.Rows()[1] << std::endl;
		//LCN::ConstMatrixBlock<LCN::Matrix4f, 3, 3> cr(ref, 0, 0);
	}

	SEPARATOR(2)
	{
		LCN::HVector3Df hv1 = { 1, 2, -1, 0 };
		LCN::HVector3Df hv2 = { 3, 4, -1, 0 };
		LCN::HVector3Df hv3;

		hv3.Vector() = hv1.Vector() ^ hv2.Vector();

		std::cout << std::endl;
	}

	SEPARATOR(3)
	{
		LCN::HVector3Df hv1 = { 1, 2, 3, 0 };
		LCN::Transpose<LCN::HVector3Df> thv1(hv1);

		std::cout << hv1 << std::endl;
		std::cout << thv1 << std::endl;

		std::cout << thv1 * hv1 << std::endl;
		std::cout << hv1 * thv1 << std::endl;
	}

	SEPARATOR(4)
	{
		LCN::Matrix4f m = {
			1, 0,  0, 1,
			0, 0, -1, 2,
			0, 1,  0, 3,
			0, 0,  0, 1
		};

		LCN::MatrixBlock<LCN::Matrix4f, 3, 3> r(m, 0, 0);
		LCN::MatrixBlock<LCN::Matrix4f, 3, 1> t(m, 0, 3);

		LCN::Matrix4f im = LCN::Matrix4f::Identity();

		LCN::MatrixBlock<LCN::Matrix4f, 3, 3> ir(im, 0, 0);
		LCN::MatrixBlock<LCN::Matrix4f, 3, 1> it(im, 0, 3);

		ir = r.Transpose();
		it = -(ir * t);

		std::cout << im << std::endl;
		std::cout << m * im << std::endl;
	}

	SEPARATOR(5)
	{
		LCN::Transform3Df T;

		std::cout << T << std::endl;

		T = {
			1, 0,  0, 1,
			0, 0, -1, 2,
			0, 1,  0, 3,
			0, 0,  0, 1
		};

		std::cout << T.RotationBlock() << std::endl;
		std::cout << T.TranslationBlock() << std::endl;

		const auto& cref(T);

		std::cout << cref.RotationBlock() << std::endl;
		std::cout << cref.TranslationBlock() << std::endl;

		auto iT = T.QuickInverse();

		std::cout << iT << std::endl;
		std::cout << T * iT << std::endl;
	}

	SEPARATOR(6)
	{
		LCN::Transform2Df R1_2 = {
			 0, 1, 4,
			-1, 0, 4,
			 0, 0, 1
		};

		LCN::HVector2Df P1 = {
			7,
			2,
			1
		};

		auto R2_1 = R1_2.QuickInverse();

		LCN::HVector2Df P2 = R2_1 * P1;

		std::cout << R2_1 << std::endl;
		std::cout << P2 << std::endl;
	}

	std::cin.get();
}