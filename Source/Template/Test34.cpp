#include <iostream>
#include <type_traits>

template <unsigned int N, typename std::enable_if<N >= 100>::type* = nullptr>
void my_function()
{
	std::cout << "N >= 100" << std::endl;
}

template <unsigned int N, typename std::enable_if<N < 100>::type* = nullptr>
void my_function()
{
	std::cout << "N < 100" << std::endl;
}

template<unsigned int N>
struct Test
{
	template<typename = void>
	void method();

	template<>
	void method<typename std::enable_if<N < 100>::type>()
	{
		std::cout << "Method < 100" << std::endl;
	}

	/*
	template<>
	void method<typename std::enable_if<N >= 100>::type>()
	{
		std::cout << "Method >= 100" << std::endl;
	}
	*/
};

int main()
{
	Test<42> test;

	test.method();
	
	/*
	Test<101> test2;

	test.method();
	*/	

	std::cin.get();
}