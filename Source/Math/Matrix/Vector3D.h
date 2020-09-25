#pragma once

#include "MatrixExpression.h"

template<typename T>
struct Vector3D : public MatrixExpression<Vector3D<T>, T>
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

	T operator()(size_t i, size_t) const { return mat[i]; }

	constexpr size_t Line()   const { return 3; }
	constexpr size_t Column() const { return 1; }
};