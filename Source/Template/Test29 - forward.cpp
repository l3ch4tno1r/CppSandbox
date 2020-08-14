#include <iostream>
#include <type_traits>

struct Test
{};

void f(Test& t)
{
	std::cout << "f(Test&)" << std::endl;
}

void f(Test&& t)
{
	std::cout << "f(Test&&)" << std::endl;
}

template<typename T>
void wrapper(T&& arg)
{
	f(std::forward<T>(arg));
}

int main()
{
	Test test;

	wrapper(test);
	wrapper(Test());

	std::cin.get();
}