#pragma once

template<class E, typename T>
class MatrixExpression
{
private:
	E& Derived() { return static_cast<E&>(*this); }
	const E& Derived() const { return static_cast<const E&>(*this); }

public:
	T operator()(size_t i, size_t j) const { return Derived()(i, j); }

	size_t Line() const { return Derived().Line(); }
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