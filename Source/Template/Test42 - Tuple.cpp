#include <iostream>
#include <tuple>
#include <type_traits>

///////////////
//-- Tuple --//
///////////////

template<typename ... _Types>
struct Tuple;

template<typename _Head, typename ... _Tail>
struct Tuple<_Head, _Tail...> : Tuple<_Tail...>
{
	using Type     = _Head;
	using BaseType = Tuple<_Tail...>;

	Type Member;

	Tuple() = default;

	template<typename _AHead, typename ... _ATail>
	Tuple(_AHead&& arg, _ATail&& ... args) :
		Member(std::forward<_AHead>(arg)),
		BaseType(std::forward<_ATail>(args)...)
	{}
};

template<>
struct Tuple<>
{};

//////////////////////
//-- TupleElement --//
//////////////////////

template<size_t _Idx, class _TupleType>
struct TupleElemByIdx;

template<size_t _Idx, typename _Head, typename ... _Tail>
struct TupleElemByIdx<_Idx, Tuple<_Head, _Tail...>>
{
	using PrevType  = TupleElemByIdx<_Idx - 1, Tuple<_Tail...>>;
	using TupleType = typename PrevType::TupleType;
	using Type      = typename PrevType::Type;
};

template<typename _Head, typename ... _Tail>
struct TupleElemByIdx<0, Tuple<_Head, _Tail...>>
{
	using TupleType = Tuple<_Head, _Tail...>;
	using Type      = _Head;
};

//////////////////////
//-- ContainsType --//
//////////////////////

template<typename _TypeToCheck, class _TupleType>
struct ContainsType;

template<typename _TypeToCheck, typename _Head, typename ... _Tail>
struct ContainsType<_TypeToCheck, Tuple<_Head, _Tail...>>
{
	using PrevType = ContainsType<_TypeToCheck, Tuple<_Tail...>>;

	static constexpr bool Value = std::is_same_v<_TypeToCheck, _Head> || PrevType::Value;
};

template<typename _TypeToCheck>
struct ContainsType<_TypeToCheck, Tuple<>>
{
	static constexpr bool Value = false;
};

/////////////////////////
//-- TupleElemByType --//
/////////////////////////

template<typename _TypeToFind, class _TupleType>
struct TupleElemByType;

template<typename _TypeToFind, typename _Head, typename ... _Tail>
struct TupleElemByType<_TypeToFind, Tuple<_Head, _Tail...>>
{
	using PrevType  = TupleElemByType<_TypeToFind, Tuple<_Tail...>>;
	using TupleType = typename PrevType::TupleType;
};

template<typename _TypeToFind, typename ... _Tail>
struct TupleElemByType<_TypeToFind, Tuple<_TypeToFind, _Tail...>>
{
	static_assert(!ContainsType<_TypeToFind, Tuple<_Tail...>>::Value, "Duplicated type.");

	using TupleType = Tuple<_TypeToFind, _Tail...>;
};


/////////////
//-- Get --//
/////////////

template<size_t _Idx, class _TupleType>
typename TupleElemByIdx<_Idx, _TupleType>::Type&
Get(_TupleType& tuple)
{
	using CurrentTupleType = typename TupleElemByIdx<_Idx, _TupleType>::TupleType;

	return static_cast<CurrentTupleType&>(tuple).Member;
}

template<typename _TypeToGet, class _TupleType>
_TypeToGet&
Get(_TupleType& tuple)
{
	using CurrentTupleType = typename TupleElemByType<_TypeToGet, _TupleType>::TupleType;

	return static_cast<CurrentTupleType&>(tuple).Member;
}

//////////////
//-- Misc --//
//////////////

using TupleTestType = Tuple<int, char, float>;

template<size_t _Idx>
using TupleElemNo = TupleElemByIdx<_Idx, TupleTestType>::TupleType;

int main()
{
	TupleTestType tuple{ 1, 'b', 3.0f };

	auto& iref = static_cast<TupleElemNo<0>&>(tuple);
	auto& cref = static_cast<TupleElemNo<1>&>(tuple);
	auto& fref = static_cast<TupleElemNo<2>&>(tuple);

	std::cout << iref.Member << ", " << cref.Member << ", " << fref.Member << '\n';

	auto& i = Get<0>(tuple);
	auto& c = Get<1>(tuple);
	auto& f = Get<2>(tuple);

	std::cout << i << ", " << c << ", " << f << '\n';

	auto& _i = Get<int>(tuple);
	auto& _c = Get<char>(tuple);
	auto& _f = Get<float>(tuple);

	std::cout << _i << ", " << _c << ", " << _f << '\n';

	std::cin.get();
}