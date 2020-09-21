#pragma once

#include <initializer_list>

#include "MatrixExpression.h"

#include "Source/ErrorHandling.h"

template<typename T, size_t L, size_t C>
class Matrix : public MatrixExpression<Matrix<T, L, C>, T>
{
public:
	using ValType = T;
	using PtrType = T*;
	using RefType = T& ;

private:
	ValType tab[L][C];

public:
	Matrix() = default;

	Matrix(const std::initializer_list<ValType>& list)
	{
		size_t Idx = 0;

		for (ValType e : list)
		{
			if (Idx > L * C)
				break;

			size_t i = Idx / C;
			size_t j = Idx % C;

			tab[i][j] = e;

			++Idx;
		}
	}

	template<class E>
	Matrix(const MatrixExpression<E, ValType>& other)
	{
		ASSERT((this->Line() == other.Line()) && (this->Column() == other.Column()));

		for (size_t i = 0; i < other.Line(); ++i)
			for (size_t j = 0; j < other.Column(); ++j)
				tab[i][j] = other(i, j);
	}

	RefType operator()(size_t i, size_t j) { return tab[i][j]; }
	ValType operator()(size_t i, size_t j) const { return tab[i][j]; }

	size_t Line() const { return L; }
	size_t Column() const { return C; }
};
