#include <iostream>

template<typename _Derived>
struct ColumnPack {};

struct TablePack {};
struct JoinClause {};
struct WhereClause {};

template<typename ... _Cols>
struct Columns : ColumnPack<Columns<_Cols...>> {};

template<typename ... _Cols>
Columns<_Cols...> Select(const _Cols&...) { return {}; }

template<typename _Columns>
struct SelectFrom {};

template<typename _Columns>
SelectFrom<_Columns> operator|(const ColumnPack<_Columns>&, const TablePack&) { return {}; }

template<typename _LHS>
struct JoinStmt {};

template<typename _LHS>
JoinStmt<JoinStmt<_LHS>> operator|(const JoinStmt<_LHS>&, const JoinClause&) { return {}; }

template<typename _Columns>
JoinStmt<SelectFrom<_Columns>> operator|(const SelectFrom< _Columns>&, const JoinClause&) { return {}; }

template<typename _LHS>
struct WhereStmt {};

template<typename _LHS>
WhereStmt<JoinStmt<_LHS>> operator|(const JoinStmt<_LHS>&, const WhereClause&) { return {}; }

template<typename _Columns>
WhereStmt<SelectFrom<_Columns>> operator|(const SelectFrom< _Columns>&, const WhereClause&) { return {}; }

int main()
{
	TablePack   f;
	JoinClause  j;
	WhereClause w;

	auto Q = Select(1, 2.0f, 'a') | f | j | j | w;

	auto test = Select(1, 2.0f, 'a');

	std::cin.get();
}