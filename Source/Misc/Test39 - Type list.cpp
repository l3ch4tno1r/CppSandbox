#include <iostream>

#include <Utilities/Source/TypeList.h>

template<typename T>
void Print()
{
	static_assert(false);
}

template<>
void Print<int>()
{
	std::cout << "int" << std::endl;
}

template<>
void Print<char>()
{
	std::cout << "char" << std::endl;
}

template<>
void Print<double>()
{
	std::cout << "double" << std::endl;
}

template<>
void Print<std::string>()
{
	std::cout << "std::string" << std::endl;
}

//--------------------//

template<class TypeList, size_t Idx = 0>
void PrintList()
{
	using CurrentType = typename LCN::Extract<Idx, TypeList>::Result;

	Print<CurrentType>();

	if constexpr (Idx + 1 != TypeList::Count())
		PrintList<TypeList, Idx + 1>();
}

int main()
{
	using TypeListToPrint = LCN::TypeList<char, int, std::string>;

	PrintList<TypeListToPrint>();

	std::cin.get();
}