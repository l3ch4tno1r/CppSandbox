#include <iostream>
#include <type_traits>

template<typename _Type, size_t _I>
struct Test
{
	void Print() const
	{
		std::cout << "General case !\n";
	}
};

template<typename _Type>
void Test<_Type, 0>::Print() const
{
	std::cout << "Hello world from 0 !\n";
}

template<typename _Type>
void Test<_Type, 1>::Print() const
{
	std::cout << "Hello world from 1 !\n";
}

int main()
{
	Test<int, 0>{}.Print();
	Test<int, 1>{}.Print();
	Test<int, 2>{}.Print();

	std::cin.get();
}