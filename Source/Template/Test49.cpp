#include <iostream>

template<typename _Derived>
struct SQLExpr
{};

template<typename _EL, typename _ER>
struct Expr : SQLExpr<Expr<_EL, _ER>>
{};

template<typename _EL, typename _ER>
Expr<_EL, _ER> operator|(const SQLExpr<_EL>&, const SQLExpr<_ER>&)
{
	return {};
}

struct SelectExpr : SQLExpr<SelectExpr>
{};

SelectExpr Select() { return {}; }

struct FromExpr : SQLExpr<FromExpr>
{};

FromExpr From() { return {}; }

struct WhereExpr : SQLExpr<WhereExpr>
{};

WhereExpr Where() { return {}; }

struct JoinExpr : SQLExpr<JoinExpr>
{};

JoinExpr Join() { return {}; }

int main()
{
	auto query1 =
		Select() | From() |
		Join() |
		Where();

	auto query2 = 
		Select() |
		Where();

	std::cin.get();
}