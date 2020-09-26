#pragma once

#include <cmath>

#include "MatrixExpression.h"

template<class Derived, typename T, size_t N>
class VectorBase : public MatrixExpression<Derived, T>
{
public:
	T operator[](size_t i) const { return this->Derived()[i]; }

	constexpr size_t Line()   const { return N; }
	constexpr size_t Column() const { return 1; }

	T Norm() const
	{
		T norm = 0;

		for (size_t i = 0; i < N; ++i)
			norm += (*this)[i] * (*this)[i];

		return std::sqrt(norm);
	}
};

template<class E, typename T, size_t N>
T operator|(const VectorBase<E, T, N>& a, const VectorBase<E, T, N>& b)
{
	T dotproduct = 0;

	for (size_t i = 0; i < N; ++i)
		dotproduct += a[i] * b[i];

	return dotproduct;
}