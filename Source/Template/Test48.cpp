#include <iostream>

template<typename _Derived>
struct Expr
{};

template<typename _LHS, typename _RHS>
struct AddExpr : Expr<AddExpr<_LHS, _RHS>>
{};

int main()
{
	std::cin.get();
}