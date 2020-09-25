#pragma once

#include "MatrixBase.h"

template<class Derived, typename T, size_t L, size_t C>
class StaticMatrixBase : public MatrixBase<Derived, T>
{
public:
	constexpr size_t Line()   const { return L; }
	constexpr size_t Column() const { return C; }

	static void AssertSquareMatrix() { static_assert(L == C, "This is not a square matrix."); }
};