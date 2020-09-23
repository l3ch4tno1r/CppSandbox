#pragma once

#include <initializer_list>

#include "MatrixBase.h"

template<typename T, size_t L, size_t C>
class Matrix : public MatrixBase<Matrix<T, L, C>, T>
{
public:
	using ValType = T;
	using PtrType = T*;
	using RefType = T& ;

private:
	ValType m_Tab[L][C];

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

			m_Tab[i][j] = e;

			++Idx;
		}
	}

	template<class E>
	Matrix(const MatrixExpression<E, ValType>& other)
	{
		ASSERT((this->Line() == other.Line()) && (this->Column() == other.Column()));

		for (size_t i = 0; i < other.Line(); ++i)
			for (size_t j = 0; j < other.Column(); ++j)
				m_Tab[i][j] = other(i, j);
	}

	template<class E>
	Matrix& operator=(const MatrixExpression<E, ValType>& other)
	{
		ASSERT((this->Line() == other.Line()) && (this->Column() == other.Column()));

		for (size_t i = 0; i < other.Line(); ++i)
			for (size_t j = 0; j < other.Column(); ++j)
				m_Tab[i][j] = other(i, j);
	}

	RefType operator()(size_t i, size_t j) { return m_Tab[i][j]; }
	ValType operator()(size_t i, size_t j) const { return m_Tab[i][j]; }

	constexpr size_t Line() const { return L; }
	constexpr size_t Column() const { return C; }

	Matrix Invert() const
	{
		Matrix<T, L, 2 * C> temp;

		for (size_t i = 0; i < L; i++)
			for (size_t j = 0; j < C; j++)
				temp(i, j) = m_Tab[i][j];

		for (size_t i = 0; i < L; i++)
			for (size_t j = C; j < 2 * C; j++)
				temp(i, j) = (i == j - C ? T(1) : T(0));

		T pseudodet = temp.GaussElimination();

		if (std::abs(pseudodet) < T(0.0001))
			throw std::exception("This matrix cannot be inverted.");

		Matrix result;

		for (size_t i = 0; i < L; i++)
			for (size_t j = 0; j < C; j++)
				result(i, j) = temp(i, j + C);

		return result;
	}
};
