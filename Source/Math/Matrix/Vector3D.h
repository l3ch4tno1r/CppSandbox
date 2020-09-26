#pragma once

#include "VectorBase.h"

template<typename T>
struct Vector3D : public VectorBase<Vector3D<T>, T, 3>
{
	union
	{
		struct
		{
			T x, y, z;
		};
		T mat[3];
	};

	Vector3D(T x, T y, T z) :
		x(x), y(y), z(z)
	{}

	template<class E>
	Vector3D(const MatrixExpression<E, T>& other)
	{
		ASSERT((this->Line() == other.Line()) && (this->Column() == other.Column()));

		for (size_t i = 0; i < 3; i++)
			mat[i] = other(i, 0);
	}

	template<class E>
	Vector3D& operator=(const MatrixExpression<E, T>& other)
	{
		ASSERT((this->Line() == other.Line()) && (this->Column() == other.Column()));

		for (size_t i = 0; i < 3; i++)
			mat[i] = other(i, 0);
	}

	T operator()(size_t i, size_t) const { return mat[i]; }
	T operator[](size_t i) const { return mat[i]; }

	constexpr size_t Line()   const { return 3; }
	constexpr size_t Column() const { return 1; }
};