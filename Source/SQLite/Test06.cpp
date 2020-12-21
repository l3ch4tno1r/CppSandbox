#include <iostream>
#include <string>

#include "Source/Memory/_MemTracker.h"

using namespace std::literals::string_literals;

#define SEPARATOR(X) std::cout << "------- " << X << " -------" << std::endl

#pragma region SQLiteExpression
//////////////////////////
//-- SQLiteExpression --//
//////////////////////////

template<class E>
class SQLiteExpression
{
public:
	// Must override
	inline size_t Size() const { return this->Derived().Size(); }

	inline char operator[](size_t i) const { return this->Derived()[i]; }

	// Methods
	std::string ToString() const
	{
		std::string result(this->Size(), '\0');

		for (size_t i = 0; i < this->Size(); ++i)
			result[i] = (*this)[i];

		return result;
	}

	inline operator std::string() const { return this->ToString(); }

protected:
	inline E& Derived() { return static_cast<E&>(*this); }
	inline const E& Derived() const { return static_cast<const E&>(*this); }
};

template<class Derived>
std::ostream& operator<<(std::ostream& ostream, const SQLiteExpression<Derived>& expr)
{
	for (size_t i = 0; i < expr.Size(); ++i)
		ostream << expr[i];

	return ostream;
}

#pragma endregion

#pragma region Entities
//////////////////
//-- Entities --//
//////////////////

template<class Derived>
class SQLiteEntity : public SQLiteExpression<Derived>
{};

///////////////
//-- Tests --//
///////////////

class QuickTest : public SQLiteEntity<QuickTest>
{
public:
	inline size_t Size() const { return 4; }

	inline char operator[](size_t i) const
	{
		static const char* const name = "Test";

		return name[i];
	}
};

class Test : public SQLiteEntity<Test>
{
public:
	Test(const char* name) :
		m_Name(name)
	{}

	inline size_t Size() const { return m_Name.size(); }

	inline char operator[](size_t i) const { return m_Name[i]; }

private:
	std::string m_Name;
};

#define CREATE_TEST(X) Test X(#X)

#pragma endregion

#pragma region Predicate

#pragma region Bases
///////////////////
//-- Predicate --//
///////////////////

template<class Derived>
class Predicate : public SQLiteExpression<Derived>
{};

///////////////////
//-- Binary Op --//
///////////////////

template<class Derived, class EL, class ER>
class BinaryPredicate : public Predicate<Derived>
{
public:
	inline size_t Size() const { return 2 + Derived::OperatorName().size() + this->el.Size() + this->er.Size(); }

	inline char operator[](size_t i) const
	{
		if (0 == i)
			return '(';

		if (this->Size() - 1 == i)
			return ')';

		size_t offseted = i - 1;

		if (offseted < this->el.Size())
			return this->el[offseted];

		offseted -= this->el.Size();

		const std::string& operatorname = Derived::OperatorName();

		if (offseted < operatorname.size())
			return operatorname[offseted];

		offseted -= operatorname.size();

		if (offseted < this->er.Size())
			return this->er[offseted];

		throw std::out_of_range("Index out of range.");
	}

protected:
	const EL& el;
	const ER& er;

	BinaryPredicate(const EL& el, const ER& er) :
		el(el),
		er(er)
	{}
};
#pragma endregion

#pragma region Comparison
////////////////////
//-- Comparison --//
////////////////////

template<class Derived, class EL, class ER>
class ComparisonStatement : public BinaryPredicate<Derived, EL, ER>
{
public:
	using Base = BinaryPredicate<Derived, EL, ER>;

protected:
	ComparisonStatement(const EL& el, const ER& er) :
		Base(el, er)
	{}
};

///////////////
//-- Equal --//
///////////////

template<class EL, class ER>
class EqualStatement : public ComparisonStatement<EqualStatement<EL, ER>, EL, ER>
{
public:
	using Base = ComparisonStatement<EqualStatement<EL, ER>, EL, ER>;

	static const std::string& OperatorName()
	{
		static std::string name(" = ");
		return name;
	}

private:
	EqualStatement(const EL& el, const ER& er) :
		Base(el, er)
	{}

	template<class _EL, class _ER>
	friend EqualStatement<_EL, _ER> operator==(const SQLiteEntity<_EL>&, const SQLiteEntity<_ER>&);
};

template<class _EL, class _ER>
EqualStatement<_EL, _ER> operator==(const SQLiteEntity<_EL>& el, const SQLiteEntity<_ER>& er)
{
	return EqualStatement<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
}

///////////////////
//-- Not Equal --//
///////////////////

template<class EL, class ER>
class NotEqualStatement : public ComparisonStatement<NotEqualStatement<EL, ER>, EL, ER>
{
public:
	using Base = ComparisonStatement<NotEqualStatement<EL, ER>, EL, ER>;

	static const std::string& OperatorName()
	{
		static std::string name(" <> ");
		return name;
	}

private:
	NotEqualStatement(const EL& el, const ER& er) :
		Base(el, er)
	{}

	template<class _EL, class _ER>
	friend NotEqualStatement<_EL, _ER> operator!=(const SQLiteEntity<_EL>&, const SQLiteEntity<_ER>&);
};

template<class _EL, class _ER>
NotEqualStatement<_EL, _ER> operator!=(const SQLiteEntity<_EL>& el, const SQLiteEntity<_ER>& er)
{
	return NotEqualStatement<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
}

/////////////////
//-- Greater --//
/////////////////

template<class EL, class ER>
class GreaterStatement : public ComparisonStatement<GreaterStatement<EL, ER>, EL, ER>
{
public:
	using Base = ComparisonStatement<GreaterStatement<EL, ER>, EL, ER>;

	static const std::string& OperatorName()
	{
		static std::string name(" > ");
		return name;
	}

private:
	GreaterStatement(const EL& el, const ER& er) :
		Base(el, er)
	{}

	template<class _EL, class _ER>
	friend GreaterStatement<_EL, _ER> operator>(const SQLiteEntity<_EL>&, const SQLiteEntity<_ER>&);
};

template<class _EL, class _ER>
GreaterStatement<_EL, _ER> operator>(const SQLiteEntity<_EL>& el, const SQLiteEntity<_ER>& er)
{
	return GreaterStatement<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
}

//////////////////////////
//-- Greater or equal --//
//////////////////////////

template<class EL, class ER>
class GreaterOrEqualStatement : public ComparisonStatement<GreaterOrEqualStatement<EL, ER>, EL, ER>
{
public:
	using Base = ComparisonStatement<GreaterOrEqualStatement<EL, ER>, EL, ER>;

	static const std::string& OperatorName()
	{
		static std::string name(" >= ");
		return name;
	}

private:
	GreaterOrEqualStatement(const EL& el, const ER& er) :
		Base(el, er)
	{}

	template<class _EL, class _ER>
	friend GreaterOrEqualStatement<_EL, _ER> operator>=(const SQLiteEntity<_EL>&, const SQLiteEntity<_ER>&);
};

template<class _EL, class _ER>
GreaterOrEqualStatement<_EL, _ER> operator>=(const SQLiteEntity<_EL>& el, const SQLiteEntity<_ER>& er)
{
	return GreaterOrEqualStatement<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
}

//////////////
//-- Less --//
//////////////

template<class EL, class ER>
class LessStatement : public ComparisonStatement<LessStatement<EL, ER>, EL, ER>
{
public:
	using Base = ComparisonStatement<LessStatement<EL, ER>, EL, ER>;

	static const std::string& OperatorName()
	{
		static std::string name(" < ");
		return name;
	}

private:
	LessStatement(const EL& el, const ER& er) :
		Base(el, er)
	{}

	template<class _EL, class _ER>
	friend LessStatement<_EL, _ER> operator<(const SQLiteEntity<_EL>&, const SQLiteEntity<_ER>&);
};

template<class _EL, class _ER>
LessStatement<_EL, _ER> operator<(const SQLiteEntity<_EL>& el, const SQLiteEntity<_ER>& er)
{
	return LessStatement<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
}

///////////////////////
//-- Less or equal --//
///////////////////////

template<class EL, class ER>
class LessOrEqualStatement : public ComparisonStatement<LessOrEqualStatement<EL, ER>, EL, ER>
{
public:
	using Base = ComparisonStatement<LessOrEqualStatement<EL, ER>, EL, ER>;

	static const std::string& OperatorName()
	{
		static std::string name(" <= ");
		return name;
	}

private:
	LessOrEqualStatement(const EL& el, const ER& er) :
		Base(el, er)
	{}

	template<class _EL, class _ER>
	friend LessOrEqualStatement<_EL, _ER> operator<=(const SQLiteEntity<_EL>&, const SQLiteEntity<_ER>&);
};

template<class _EL, class _ER>
LessOrEqualStatement<_EL, _ER> operator<=(const SQLiteEntity<_EL>& el, const SQLiteEntity<_ER>& er)
{
	return LessOrEqualStatement<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
}

#pragma endregion

#pragma region Logical
/////////////////
//-- Logical --//
/////////////////

template<class Derived, class EL, class ER>
class LogicalStatement : public BinaryPredicate<Derived, EL, ER>
{
public:
	using Base = BinaryPredicate<Derived, EL, ER>;

protected:
	LogicalStatement(const EL& el, const ER& er) :
		Base(el, er)
	{}
};

/////////////////////
//-- Logical and --//
/////////////////////

template<class EL, class ER>
class AndStatement : public LogicalStatement<AndStatement<EL, ER>, EL, ER>
{
public:
	using Base = LogicalStatement<AndStatement<EL, ER>, EL, ER>;

	static const std::string& OperatorName()
	{
		static std::string name(" and ");
		return name;
	}

private:
	AndStatement(const EL& el, const ER& er) :
		Base(el, er)
	{}

	template<class _EL, class _ER>
	friend AndStatement<_EL, _ER> operator&&(const Predicate<_EL>&, const Predicate<_ER>&);
};

template<class _EL, class _ER>
AndStatement<_EL, _ER> operator&&(const Predicate<_EL>& el, const Predicate<_ER>& er)
{
	return AndStatement<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
}
#pragma endregion

#pragma endregion

int main()
{
	/*
	SEPARATOR(1);
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		QuickTest test1;
		QuickTest test2;
		QuickTest test3;

		auto equals =
			test1 == test2 &&
			test2 >  test1 &&
			test1 != test1 &&
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
	*/

	SEPARATOR(3);
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		CREATE_TEST(FirstName);
		CREATE_TEST(LastName);

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

		CREATE_TEST(FirstName);
		CREATE_TEST(LastName);

		std::cout << (FirstName == LastName) << std::endl;
		std::cout << (FirstName != LastName) << std::endl;
		std::cout << (FirstName < LastName) << std::endl;
		std::cout << (FirstName <= LastName) << std::endl;
		std::cout << (FirstName > LastName) << std::endl;
		std::cout << (FirstName >= LastName) << std::endl;
	}

	std::cin.get();
}
