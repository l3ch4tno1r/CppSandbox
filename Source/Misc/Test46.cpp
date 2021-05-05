#include <iostream>
#include <type_traits>

template<typename T>
T Adder(T arg0)
{
	return arg0;
}

template<typename T, typename ...Args>
T Adder(T arg0, Args ... args)
{
	static_assert(std::conjunction_v<std::is_same<T, Args>...>);
	return arg0 + Adder(args...);
}

int main()
{
	//std::cout << Adder(1, 2.0f, 3u) << std::endl;
	std::cout << Adder(1, 2, 3) << std::endl;
	std::cout << Adder(4.0f, 5.0f) << std::endl;

	std::cin.get();
}