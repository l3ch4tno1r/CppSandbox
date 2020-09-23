#pragma once

#include "Source/ErrorHandling.h"

#pragma region MatrixExpression
///////////////////////////
//-- Matrix Expression --//
///////////////////////////

template<class E, typename T>
class MatrixExpression
{
protected:
	inline E& Derived() { return static_cast<E&>(*this); }
	inline const E& Derived() const { return static_cast<const E&>(*this); }

public:
	T operator()(size_t i, size_t j) const { return Derived()(i, j); }

	size_t Line()   const { return Derived().Line(); }
	size_t Column() const { return Derived().Column(); }
};

template<class E, typename T>
std::ostream& operator<<(std::ostream& stream, const MatrixExpression<E, T>& mat)
{
	for (size_t i = 0; i < mat.Line(); ++i)
	{
		for (size_t j = 0; j < mat.Column(); ++j)
			stream << mat(i, j) << ' ';

		stream << std::endl;
	}

	return stream;
}
#pragma endregion

#pragma region MatrixOperations
///////////////////////////
//-- Matrix Operations --//
///////////////////////////

#pragma region Addition
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
#pragma endregion

#pragma region Substraction
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
#pragma endregion

#pragma region Multiplication
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
#pragma endregion

#pragma endregion