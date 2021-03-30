#include <iostream>
#include <string>
#include <tuple>

#include <Utilities/Source/TypeList.h>

template<typename T1, typename T2>
void PrintPair(const T1& a1, const T2& a2)
{
	std::cout << a1 << ", " << a2 << std::endl;
}

//----------------//

// prints every element of a tuple
template<size_t Idx = 0, typename... Tp>
void Print(std::tuple<Tp...>& t)
{
	const size_t size = sizeof...(Tp);

	const size_t I = Idx / size;
	const size_t J = Idx % size;

	if constexpr (I <= J)
	{
		PrintPair(std::get<I>(t), std::get<J>(t));
		//std::cout << '(' << I << ", " << J << ") ";

		if constexpr (J == size - 1)
			std::cout << std::endl;
	}

	if constexpr (Idx + 1 != size * size)
		Print<Idx + 1>(t);
}

template<typename typelist, typename tuple, size_t Idx = 0>
void Print2(const tuple& t)
{
	using ResultType = typename LCN::Extract<Idx, typelist>::Result;

	std::cout << std::get<ResultType>(t) << std::endl;

	if constexpr (Idx + 1 != typelist::Count())
		Print2<typelist, tuple, Idx + 1>(t);
}

int main()
{
	using TypeListTest = LCN::TypeList<float, int, std::string>;
	using TupleType    = std::tuple<int, float, std::string>;

	TupleType t1 = { 1, 2.0f, "Hello world" };

	//Print(t1);
	Print2<TypeListTest>(t1);

	std::cin.get();
}