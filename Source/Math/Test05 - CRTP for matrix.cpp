#include <iostream>

template<class E, typename T>
class MatrixExpression
{
public:
	T& operator()(size_t i, size_t j) { return static_cast<E&>(*this)(i, j); }
};

int main()
{
	std::cin.get();
}