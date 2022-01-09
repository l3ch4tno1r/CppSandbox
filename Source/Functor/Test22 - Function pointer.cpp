#include <iostream>

template<class F>
struct Func;

template<typename Ret, typename ... Args>
struct Func<Ret(Args...)>
{
	using FctPtr = Ret(*)(Args...);
};

template<typename Ret, typename ... Args>
using FctPtr = Func<Ret(Args...)>::FctPtr;

int main()
{
	Func<void(int)>::FctPtr ptr1 = nullptr;
	FctPtr<void(int)> ptr2 = nullptr;

	std::cin.get();
}