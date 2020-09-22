#pragma once

template<class E, typename T>
class MatrixExpression
{
#define MATRIXEXPRESSIONINTERFACE \
public:\
	T operator()(size_t i, size_t j) const { return static_cast<const E&>(*this)(i, j); }\
	size_t Line()   const { return static_cast<const E&>(*this).Line(); }\
	size_t Column() const { return static_cast<const E&>(*this).Column(); }

	MATRIXEXPRESSIONINTERFACE
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