#pragma once

#include <algorithm>

#include "MatrixExpression.h"

template<class Derived, typename T>
class MatrixBase : public MatrixExpression<Derived, T>
{
public:
	T& operator()(size_t i, size_t j) { return this->Derived()(i, j); }

	/////////////////
	//-- Methods --//
	/////////////////

	void SwapLines(size_t i, size_t j)
	{
		if (i >= this->Line() || j >= this->Line())
			throw std::out_of_range("Index out of range.");

		T temp;

		for (int k = 0; k < this->Column(); k++)
		{
			temp = this->Derived()(i, k);

			this->Derived()(i, k) = this->Derived()(j, k);
			this->Derived()(j, k) = temp;
		}
	}

	void ScaleLine(size_t idx, T scalefactor)
	{
		if (idx >= this->Line())
			throw std::out_of_range("Index out of range.");

		for (size_t j = 0; j < this->Column(); j++)
			this->Derived()(idx, j) *= scalefactor;
	}

	void CombineLines(size_t idx1, T factor1, size_t idx2, T factor2)
	{
		if (idx1 >= this->Line() || idx2 >= this->Line())
			throw std::out_of_range("Index out of range.");

		for (size_t j = 0; j < this->Column(); j++)
			this->Derived()(idx1, j) = factor1 * this->Derived()(idx1, j) + factor2 * this->Derived()(idx2, j);
	}

	T GaussElimination()
	{
		size_t linepivot    = 0;
		size_t permutations = 0;
		T      pseudodet(1);

		size_t L = this->Line();
		size_t C = this->Column();

		for (size_t j = 0; j < std::min(L, C); j++)
		{
			// Recherche du pivot
			T   max = 0;
			int maxpos = 0;

			for (size_t i = linepivot; i < L; i++)
			{
				if (std::abs(this->Derived()(i, j)) > max)
				{
					max = abs(this->Derived()(i, j));
					maxpos = i;
				}
			}

			// maxpos est le pivot
			if (this->Derived()(maxpos, j) == 0)
				return T(0);

			pseudodet *= this->Derived()(maxpos, j);

			ScaleLine(maxpos, T(1) / (this->Derived()(maxpos, j)));

			if (maxpos != j)
			{
				SwapLines(maxpos, linepivot);
				permutations++;
			}

			for (size_t i = 0; i < L; i++)
				if (i != linepivot)
					CombineLines(i, T(1), linepivot, -(this->Derived()(i, j)));

			linepivot++;
		}

		return (permutations % 2 == 0 ? T(1) : T(-1)) * pseudodet;
	}

	////////////////////////////////////////
	//-- Square matrix specific methods --//
	////////////////////////////////////////

	bool IsSquareMatrix() const { return this->Line() == this->Column(); }

	T Trace() const
	{
		if (!this->IsSquareMatrix())
			throw std::exception("This is not a square matrix.");

		T result(T(0));

		for (size_t i = 0; i < this->Line(); i++)
			result += this->Derived()(i, i);

		return result;
	}

	T Det() const
	{
		if (!this->IsSquareMatrix())
			throw std::exception("This is not a square matrix.");

		Derived temp = *this;

		return temp.GaussElimination();
	}

	Derived Invert() const
	{
		auto temp = Derived::Matrix2C();

		if (this->Line() != temp.Line() || 2 * this->Column() != temp.Column())
			throw std::exception("Line / Column number not matching.");

		size_t L = this->Line();
		size_t C = this->Column();

		for (size_t i = 0; i < L; i++)
			for (size_t j = 0; j < C; j++)
				temp(i, j) = this->Derived()(i, j);

		for (size_t i = 0; i < L; i++)
			for (size_t j = C; j < 2 * C; j++)
				temp(i, j) = (i == j - C ? T(1) : T(0));

		T pseudodet = temp.GaussElimination();

		if (std::abs(pseudodet) < T(0.0001))
			throw std::exception("This matrix cannot be inverted.");

		Derived result;

		for (size_t i = 0; i < L; i++)
			for (size_t j = 0; j < C; j++)
				result(i, j) = temp(i, j + C);

		return result;
	}
};