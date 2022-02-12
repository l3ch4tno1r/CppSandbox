#include <iostream>

////////////////
//-- Select --//
////////////////

struct SelectBloc {};

SelectBloc Select() { return {}; }

template<typename _NextStmt>
struct SelectStmt {};

template<typename _Derived>
struct AfterSelect {};

template<typename _NextStmt>
SelectStmt<_NextStmt> operator|(const SelectBloc&, const AfterSelect<_NextStmt>&) { return {}; }

//////////////
//-- From --//
//////////////

struct FromClause : AfterSelect<FromClause> {};

FromClause From() { return {}; }

template<typename _NextStmt>
struct FromStmt : AfterSelect<FromStmt<_NextStmt>> {};

template<typename _Derived>
struct AfterFrom {};

template<typename _NextStmt>
FromStmt<_NextStmt> operator|(const FromClause&, const AfterFrom<_NextStmt>&) { return {}; }

//////////////
//-- Join --//
//////////////

struct JoinStmt : AfterFrom<JoinStmt> {};

JoinStmt Join() { return {}; }

int main()
{
	auto query =
		Select() |
		From() |
		Join();

	std::cin.get();
}