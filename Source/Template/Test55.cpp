#include <iostream>

template<typename _Type1, typename _Type2>
struct ReturnType
{
	using Type = typename ReturnType<_Type2, _Type1>::Type;
};

template<typename _Type>
struct ReturnType<_Type, _Type>
{
	using Type = _Type;
};

//----------------------//

template<>
struct ReturnType<int, float>
{
	using Type = float;
};

template<typename _Type1, typename _Type2>
typename
ReturnType<_Type1, _Type2>::Type
Function(
	_Type1 a1,
	_Type2 a2)
{
	return Function(a2, a1);
}

float Function(int i, float f)
{
	return f + i;
}

int main()
{
	auto r1 = Function(1.0f, 2);
	auto r2 = Function(1, 2.0f);

	std::cin.get();
}