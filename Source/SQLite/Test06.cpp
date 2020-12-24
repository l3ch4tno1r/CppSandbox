#include <iostream>
#include <string>
#include <functional>

#include "Source/Memory/_MemTracker.h"

#include "SQLiteEntity/Expression.h"

using namespace std::literals::string_literals;
using namespace LCN::SQLiteEntity;

#define SEPARATOR(X) std::cout << "------- " << X << " -------" << std::endl

#pragma region Expression
//////////////////////////
//-- SQLiteExpression --//
//////////////////////////

template<class Derived>
std::ostream& operator<<(std::ostream& ostream, const Expression<Derived>& expr)
{
	for (size_t i = 0; i < expr.Size(); ++i)
		ostream << expr[i];

	return ostream;
}

#pragma endregion

#pragma region Entities

///////////////
//-- Tests --//
///////////////

class QuickTest : public Variable<QuickTest>
{
public:
	inline size_t Size() const { return 4; }

	inline char operator[](size_t i) const
	{
		static const char* const name = "Test";

		return name[i];
	}
};

template<class Obj>
class Column : public Variable<Column<Obj>>
{
public:
	Column(const char* name) :
		m_Name(name)
	{}

	inline size_t Size() const { return 1 + Obj::Name().size() + m_Name.size(); }

	inline char operator[](size_t i) const
	{
		size_t i_off = 0;

		if (i < Obj::Name().size())
			return Obj::Name()[i];

		i -= Obj::Name().size();

		if (0 == i)
			return '.';

		--i;

		if(i < m_Name.size())
			return m_Name[i];

		throw std::out_of_range("Index out of range.");
	}

private:
	std::string m_Name;
};

template<class C>
class Entity;

#define CONSTRUCT_FIELD(X) X(*this, #X)

class Contact
{
public:
	const std::string& FirstName() const { return m_FirstName; }
	std::string& FirstName() { return m_FirstName; }

	const std::string& LastName() const { return m_LastName; }
	std::string& LastName() { return m_LastName; }

private:
	std::string m_FirstName;
	std::string m_LastName;
};

template<>
class Entity<Contact>
{
public:
	using ColumnType = Column<Entity<Contact>>;

	static ColumnType FirstName;
	static ColumnType LastName;

	static const std::string& Name()
	{
		static std::string name = "Contact";
		return name;
	}
};

using eContact = Entity<Contact>;

eContact::ColumnType eContact::FirstName("FirstName");
eContact::ColumnType eContact::LastName("LastName");

#pragma endregion

int main()
{
	SEPARATOR(1);
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		QuickTest test1;
		QuickTest test2;
		QuickTest test3;

		auto equals =
			test1 == test2 &&
			test2 >  test1 &&
			test1 != test1 ||
			test2 >= test3 &&
			test3 <  test1 &&
			test1 <= test2;

		//auto equals = test1 == test2;

		std::string str = equals;

		std::cout << str << std::endl;
	}

	SEPARATOR(2);
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		QuickTest test1, test2;

		std::cout << (test1 == test2) << std::endl;
		std::cout << (test1 != test2) << std::endl;
		std::cout << (test1 <  test2) << std::endl;
		std::cout << (test1 <= test2) << std::endl;
		std::cout << (test1 >  test2) << std::endl;
		std::cout << (test1 >= test2) << std::endl;
	}

	/*
	SEPARATOR(3);
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		CREATE_FIELD(FirstName);
		CREATE_FIELD(LastName);

		std::cout << (FirstName == LastName) << std::endl;
		std::cout << (FirstName != LastName) << std::endl;
		std::cout << (FirstName <  LastName) << std::endl;
		std::cout << (FirstName <= LastName) << std::endl;
		std::cout << (FirstName >  LastName) << std::endl;
		std::cout << (FirstName >= LastName) << std::endl;
	}

	SEPARATOR(3.1);
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		CREATE_FIELD(FirstName);
		CREATE_FIELD(LastName);

		std::cout << (FirstName == LastName) << std::endl;
		std::cout << (FirstName != LastName) << std::endl;
		std::cout << (FirstName <  LastName) << std::endl;
		std::cout << (FirstName <= LastName) << std::endl;
		std::cout << (FirstName >  LastName) << std::endl;
		std::cout << (FirstName >= LastName) << std::endl;
	}

	SEPARATOR(4);
	{
		CREATE_FIELD(Col1);
		CREATE_FIELD(Col2);
		CREATE_FIELD(Col3);

		std::cout << (Col1 == Col2 && Col2 == Col3 && Col3 == Col1) << std::endl;
		std::cout << (Col1 == Col2 && (Col2 == Col3 && Col3 == Col1)) << std::endl;
		std::cout << (Col1 == Col2 || Col2 == Col3 && Col3 == Col1) << std::endl;
		std::cout << (Col1 == Col2 && Col2 == Col3 || !(Col3 == Col1)) << std::endl;
	}
	*/

	SEPARATOR(3);
	{
		std::cout << (eContact::FirstName > eContact::LastName) << std::endl;
	}

	std::cin.get();
}
