#pragma once

#include "MatrixExpression.h"

#include "Source/ErrorHandling.h"

//////////////////
//-- Addition --//
//////////////////

template<class EL, class ER, typename T>
class MatrixAdd : public MatrixExpression<MatrixAdd<EL, ER, T>, T>
{
private:
	const EL& el;
	const ER& er;

	MatrixAdd(const EL& el, const ER& er) :
		el(el),
		er(er)
	{}

	template<class EL, class ER, typename T>
	friend MatrixAdd<EL, ER, T> operator+(const MatrixExpression<EL, T>& el, const MatrixExpression<ER, T>& er);

public:

	T operator()(size_t i, size_t j) const { return el(i, j) + er(i, j); }

	size_t Line()   const { return el.Line(); }
	size_t Column() const { return el.Column(); }
};

template<class EL, class ER, typename T>
MatrixAdd<EL, ER, T> operator+(const MatrixExpression<EL, T>& el, const MatrixExpression<ER, T>& er)
{
	ASSERT((el.Line() == er.Line()) && (el.Column() == er.Column()));

	return MatrixAdd<EL, ER, T>(static_cast<const EL&>(el), static_cast<const ER&>(er));
}

//////////////////////
//-- Substraction --//
//////////////////////

template<class EL, class ER, typename T>
class MatrixSub : public MatrixExpression<MatrixSub<EL, ER, T>, T>
{
private:
	const EL& el;
	const ER& er;

	MatrixSub(const EL& el, const ER& er) :
		el(el),
		er(er)
	{}

	template<class EL, class ER, typename T>
	friend MatrixSub<EL, ER, T> operator-(const MatrixExpression<EL, T>& el, const MatrixExpression<ER, T>& er);

public:

	T operator()(size_t i, size_t j) const { return el(i, j) - er(i, j); }

	size_t Line()   const { return el.Line(); }
	size_t Column() const { return el.Column(); }
};

template<class EL, class ER, typename T>
MatrixSub<EL, ER, T> operator-(const MatrixExpression<EL, T>& el, const MatrixExpression<ER, T>& er)
{
	ASSERT((el.Line() == er.Line()) && (el.Column() == er.Column()));

	return MatrixSub<EL, ER, T>(static_cast<const EL&>(el), static_cast<const ER&>(er));
}

////////////////////////
//-- Multiplication --//
////////////////////////

template<class EL, class ER, typename T>
class MatrixMul : public MatrixExpression<MatrixMul<EL, ER, T>, T>
{
private:
	const EL& el;
	const ER& er;

	MatrixMul(const EL& el, const ER& er) :
		el(el),
		er(er)
	{}

	template<class EL, class ER, typename T>
	friend MatrixMul<EL, ER, T> operator*(const MatrixExpression<EL, T>& el, const MatrixExpression<ER, T>& er);

public:

	T operator()(size_t i, size_t j) const
	{
		T result = 0;

		for (size_t k = 0; k < el.Column(); ++k)
			result += el(i, k) * er(k, j);

		return result;
	}

	size_t Line()   const { return el.Line(); }
	size_t Column() const { return er.Column(); }
};

template<class EL, class ER, typename T>
MatrixMul<EL, ER, T> operator*(const MatrixExpression<EL, T>& el, const MatrixExpression<ER, T>& er)
{
	ASSERT(el.Column() == er.Line());

	return MatrixMul<EL, ER, T>(static_cast<const EL&>(el), static_cast<const ER&>(er));
}