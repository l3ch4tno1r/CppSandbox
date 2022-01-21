#include <iostream>
#include <vector>

template<class _Derived>
struct Traits;

///////////////////
//-- RangeExpr --//
///////////////////

template<class _Derived>
class RangeExpr
{
public:
	using IteratorType = typename Traits<_Derived>::IteratorType;

	IteratorType begin() const { return this->Derived().begin(); }
	IteratorType end()   const { return this->Derived().end(); }

private:
	const _Derived& Derived() const { return static_cast<const _Derived&>(*this); }
		  _Derived& Derived()		{ return static_cast<	   _Derived&>(*this); }
};

//////////////////
//-- FromExpr --//
//////////////////

template<class _ContainerType>
class FromExpr : public RangeExpr<FromExpr<_ContainerType>>
{
public:
	using IteratorType = typename _ContainerType::const_iterator;

	template<class _ContainerTypeArg>
	friend FromExpr<_ContainerTypeArg> From(const _ContainerTypeArg&);

	inline IteratorType begin() const { return m_Target.begin(); }
	inline IteratorType end()   const { return m_Target.end();   }

private:
	FromExpr(const _ContainerType& container) :
		m_Target{container}
	{}

private:
	const _ContainerType& m_Target;
};

template<class _ContainerType>
struct Traits<FromExpr<_ContainerType>>
{
	using IteratorType = typename _ContainerType::const_iterator;
};

template<class _ContainerTypeArg>
FromExpr<_ContainerTypeArg> From(const _ContainerTypeArg& container)
{
	return { container };
}

////////////////////////
//-- WherePredicate --//
////////////////////////

template<typename _PredType>
class WherePredicate
{
public:
	template<typename _PredTypeArg>
	friend WherePredicate<_PredTypeArg> Where(_PredTypeArg predicate);

	template<typename ... _Args>
	inline bool operator()(_Args ... args) const
	{
		return m_Predicate(args...);
	}

private:
	WherePredicate(_PredType predicate) :
		m_Predicate{ predicate }
	{}

private:
	_PredType m_Predicate;
};

template<typename _PredTypeArg>
WherePredicate<_PredTypeArg> Where(_PredTypeArg predicate)
{
	return { predicate };
}

///////////////////
//-- WhereExpr --//
///////////////////

template<class _RangeExprType, class _WherePredType>
class WhereExprIterator
{
private:
	using RangeIteratorType = typename Traits<_RangeExprType>::IteratorType;

private:
	RangeIteratorType m_Iterator;
	const _WherePredType& m_Predicate;
};

template<class _RangeExprType, class _WherePredType>
class WhereExpr : public RangeExpr<WhereExpr<_RangeExprType, _WherePredType>>
{
public:
	template<class _RangeExprArg, class _WherePredArg>
	friend WhereExpr<_RangeExprArg, _WherePredArg> operator|(const _RangeExprArg&, const _WherePredArg&);

	using IteratorType = WhereExprIterator<_RangeExprType, _WherePredType>;

	IteratorType begin() const { return { m_Range.begin(), m_Where }; }
	IteratorType end()   const { return { m_Range.end(),   m_Where }; }

private:
	WhereExpr(const _RangeExprType& range, const _WherePredType& where) :
		m_Range{ range },
		m_Where{ where }
	{}

private:
	const _RangeExprType& m_Range;
	const _WherePredType& m_Where;
};

template<class _RangeExprArg, class _WherePredArg>
WhereExpr<_RangeExprArg, _WherePredArg> operator|(const _RangeExprArg& range, const _WherePredArg& where)
{
	return { range, where };
}

template<class _RangeExprType, class _WherePredType>
struct Traits<WhereExpr<_RangeExprType, _WherePredType>>
{
	using IteratorType = void;
};

//////////////
//-- Main --//
//////////////

int main()
{
	std::vector<int> vec{ 1, 2, 0, 8, 5, 3, 1, 4 };

	auto range = From(vec)
		| Where([](int i)
		{
			return i % 2 == 0;
		});

	for (const auto& i : range)
		std::cout << i << ' ';

	std::cin.get();
}