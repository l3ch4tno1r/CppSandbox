#include <iostream>

#include "LCN_Math/Source/Geometry/Vector.h"
#include "LCN_Math/Source/Geometry/Transform.h"

#define SEPARATOR(X) std::cout << "-------- " << X << " --------" << std::endl
#define DISPLAY(X) std::cout << #X << " = " << X << std::endl;

template<class Subject>
class SmallMatrix;

template<class _Derived, typename T>
class Matrix
{
public:
	using ValType  = T;
	using PtrType  = ValType*;
	using RefType  = ValType&;
	using ViewType = typename SmallMatrix<_Derived>;

	ValType operator()(int i, int j) const { return Derived()(i, j); }

	int Line()   const { return Derived().Line(); }
	int Column() const { return Derived().Column(); }

	ViewType View(int ioffset, int joffset) const { return ViewType(Derived(), ioffset, joffset); }

protected:
	_Derived& Derived() { return static_cast<_Derived&>(*this); }
	const _Derived& Derived() const { return static_cast<const _Derived&>(*this); }
};

template<class _Derived, typename T>
std::ostream& operator<<(std::ostream& stream, const Matrix<_Derived, T>& mat)
{
	for (int i = 0; i < mat.Line(); i++)
	{
		for (int j = 0; j < mat.Column(); j++)
		{
			stream << mat(i, j) << ' ';
		}

		stream << '\n';
	}

	return stream;
}

template<class Subject>
class SmallMatrix : public Matrix<SmallMatrix<Subject>, typename Subject::ValType>
{
public:
	using ValType = typename Subject::ValType;

	SmallMatrix(const Subject& bigm, int ioffset, int joffset) :
		m_Ref(bigm),
		m_Ioffset(ioffset),
		m_Joffset(joffset)
	{}

	ValType operator()(int i, int j) const { return m_Ref(i + m_Ioffset, j + m_Joffset); }

	int Line()   const { return 2; }
	int Column() const { return 2; }

private:
	const Subject& m_Ref;
	int m_Ioffset;
	int m_Joffset;
};

template<typename T>
class BigMatrix : public Matrix<BigMatrix<T>, T>
{
public:
	using ValType  = T;
	using ViewType = SmallMatrix<BigMatrix>;

	BigMatrix(const std::initializer_list<ValType>& list)
	{
		int idx = 0;

		for(auto e : list)
		{
			int i = idx / 3;
			int j = idx % 3;

			m_Mat[i][j] = e;

			++idx;
		}
	}

	ValType operator()(int i, int j) const { return m_Mat[i][j]; }

	int Line()   const { return 3; }
	int Column() const { return 3; }

private:
	ValType m_Mat[3][3];
};

int main()
{
	SEPARATOR("Misc");
	{
		Vector3Df vec1 = { 1, 2, 3 };
		Vector3Df vec2 = { 4, 5, 6 };
		Vector3Df vec3 = vec1 + vec2;

		//vec3 = vec1 + vec2;

		std::cout << vec3 << std::endl;
		std::cout << (vec1 | vec2) << std::endl;

		SEPARATOR("Vector 2D");

		Vector2Df pvec = { 1, 2 };

		std::cout << "x = " << pvec.x() << std::endl;
		std::cout << "y = " << pvec.y() << std::endl;
		//std::cout << "w = " << pvec.w() << std::endl;
		//std::cout << "z = " << pvec.z() << std::endl;

		SEPARATOR("Homogeneous vector 3D");

		HVector3Df hvec1 = { 4, 2, 3, 1 };

		std::cout << "x = " << hvec1.x() << std::endl;
		std::cout << "y = " << hvec1.y() << std::endl;
		std::cout << "z = " << hvec1.z() << std::endl;
		std::cout << "w = " << hvec1.w() << std::endl;
		std::cout << "Lines = " << hvec1.Line() << std::endl;

		SEPARATOR("Homogeneous vector 2D");

		HVector2Df hpvec = { 3, 4, 1 };

		std::cout << "x = " << hpvec.x() << std::endl;
		std::cout << "y = " << hpvec.y() << std::endl;
		std::cout << "w = " << hpvec.w() << std::endl;

		SEPARATOR(1);

		std::cout << std::boolalpha;

		DISPLAY(Vector2Df::cm_X_Accessible);
		DISPLAY(Vector2Df::cm_Y_Accessible);
		DISPLAY(Vector2Df::cm_Z_Accessible);
		DISPLAY(Vector2Df::cm_W_Accessible);

		SEPARATOR(2);

		DISPLAY(HVector2Df::cm_X_Accessible);
		DISPLAY(HVector2Df::cm_Y_Accessible);
		DISPLAY(HVector2Df::cm_Z_Accessible);
		DISPLAY(HVector2Df::cm_W_Accessible);

		SEPARATOR(3);

		DISPLAY(Vector3Df::cm_X_Accessible);
		DISPLAY(Vector3Df::cm_Y_Accessible);
		DISPLAY(Vector3Df::cm_Z_Accessible);
		DISPLAY(Vector3Df::cm_W_Accessible);

		SEPARATOR(4);

		DISPLAY(HVector3Df::cm_X_Accessible);
		DISPLAY(HVector3Df::cm_Y_Accessible);
		DISPLAY(HVector3Df::cm_Z_Accessible);
		DISPLAY(HVector3Df::cm_W_Accessible);
	}

	SEPARATOR("Cross product");
	{
		Vector3Df vec1 = { 1, 2, 3 };
		Vector3Df vec2 = { 4, 5, 6 };

		//Vector3Df cprod = vec1 ^ vec2;
		//
		//std::cout << cprod << std::endl;

		HVector3Df hvec1 = { 1, 2, 3, 1 };
		HVector3Df hvec2 = { 4, 5, 6, 1 };

		//HVector3Df hcprod = hvec1 ^ hvec2; // Will fail on purpose
	}

	SEPARATOR("Homogeneous");
	{
		Vector3Df vec1 = { 1, 2, 3 };
		auto hvec1 = vec1.Homogeneous(0);

		std::cout << hvec1 << std::endl;
	}

	SEPARATOR("Operators");
	{
		Vector3Df vec1 = { 1, 2, 3 };
		Vector3Df vec2 = { 4, 5, 6 };

		auto sum = vec1 + vec2 + vec1;

		std::cout << sum << std::endl;

		auto diff = vec2 - vec1;

		std::cout << diff << std::endl;

		float dotp1 = (Vector3Df)sum | (Vector3Df)diff;

		std::cout << dotp1 << std::endl;
	}

	SEPARATOR("Matrix view");
	{
		StaticMatrix<float, 3, 3> mat = {
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		};

		std::cout << mat << std::endl;

		auto view = mat.View<2, 2>(0, 1);

		std::cout << view << std::endl;

		Vector2Df vec = { 1, 2 };

		std::cout << view * vec << std::endl;

		HVector3Df hvec = { 1, 2, 3, 1 };

		HVector3Df::RVectorView svecview(hvec, 0, 0);
		Vector3Df svec = hvec.View<3, 1>(0, 0);

		std::cout << svecview << std::endl;
		std::cout << svec << std::endl;

		//Vector3Df crossp = svec ^ svecview;
		//
		//std::cout << crossp << std::endl;

		HVector3Df hvec2 = { 4, 5, 6, 1 };
		HVector3Df::RVectorView svecview2 = hvec2.VectorView();

		std::cout << svecview2 << std::endl;
	}

	SEPARATOR("Test");
	{
		BigMatrix<float> mat = {
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		};

		std::cout << mat << std::endl;

		BigMatrix<float>::ViewType view = mat.View(1, 0);

		std::cout << view << std::endl;
	}

	SEPARATOR("Transform");
	{
		Transform3Df transform1 = {
			1, 0, 0, 1,
			0, 0, 1, 2,
			0, 1, 0, 3,
			0, 0, 0, 1
		};

		std::cout << transform1 << std::endl;
		std::cout << transform1[3] << std::endl;

		Transform3Df transform2;

		std::cout << transform2 << std::endl;

		Transform3Df itransform1 = transform1.Invert();
		auto product = transform1 * itransform1;

		std::cout << itransform1 << std::endl;
		std::cout << transform1 * itransform1 << std::endl;

		HVector3Df hvec = { 4, 5, 6, 1 };

		std::cout << transform1 * hvec << std::endl;
	}

	SEPARATOR("Traits");
	{
		Traits<HVector3Df>::ValType t = 0.0f;
		Traits<Transform3Df>::LineAtCT;
	}

	std::cin.get();
}