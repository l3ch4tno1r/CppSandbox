#include <iostream>
#include <type_traits>

template<typename _RHS>
struct LHS {};

struct C {};
struct B : LHS<C> {};
struct A : LHS<B> {};

template<typename _L, typename _R>
struct R {};

template<typename _L>
struct R<_L, B> : LHS<C> {};

template<typename _L, typename _R>
std::enable_if_t<std::is_base_of_v<LHS<_R>, _L>, R<_L, _R>>
operator|(const _L&, const _R&)
{
	return {};
}

int main()
{
	A a;
	B b;
	C c;

	auto result = a | b | c;

	std::cin.get();
}