#include <iostream>
#include <string>

#include "Source/Memory/_MemTracker.h"

using namespace std::literals::string_literals;

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

//////////////////
//-- Entities --//
//////////////////

template<class Derived>
class SQLiteEntity : public SQLiteExpression<Derived>
{};

class Test : public SQLiteEntity<Test>
{
public:
	inline size_t Size() const { return 4; }

	inline char operator[](size_t i) const
	{
		static const char* const name = "Test";

		return name[i];
	}
};

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
protected:
	const EL& el;
	const ER& er;

	BinaryPredicate(const EL& el, const ER& er) :
		el(el),
		er(er)
	{}
};

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

	inline size_t Size() const { return 5 + this->el.Size() + this->er.Size(); }

	inline char operator[](size_t i) const
	{
		static const char* const equalstr = " = ";
		
		if (0 == i)
			return '(';

		if (this->Size() - 1 == i)
			return ')';

		size_t offseted = i - 1;

		if (offseted < this->el.Size())
			return this->el[offseted];

		offseted -= this->el.Size();

		if (offseted < 3)
			return equalstr[offseted];

		offseted -= 3;

		if (offseted < this->er.Size())
			return this->er[offseted];
	}

	//std::string ToString() const { return "("s + this->el.ToString() + " = " + this->er.ToString() + ')'; }

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

	inline size_t Size() const { return 7 + this->el.Size() + this->er.Size(); }

	inline char operator[](size_t i) const
	{
		static const char* const andstr = " and ";

		if (0 == i)
			return '(';

		if (this->Size() - 1 == i)
			return ')';

		size_t offseted = i - 1;

		if (offseted < this->el.Size())
			return this->el[offseted];

		offseted -= this->el.Size();

		if (offseted < 5)
			return andstr[offseted];

		offseted -= 5;

		if (offseted < this->er.Size())
			return this->er[offseted];
	}

	std::string ToString() const { return "("s + this->el.ToString() + " and " + this->er.ToString() + ')'; }

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

int main()
{
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		Test test1;
		Test test2;

		auto equals =
			test1 == test2 &&
			test2 == test1 &&
			test1 == test1;

		//auto equals = test1 == test2;

		std::string str = equals;

		std::cout << str << std::endl;
	}

	std::cin.get();
}
