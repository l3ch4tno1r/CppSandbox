#include <iostream>
#include <functional>
#include <future>

#include "Utilities/InstanceCounter.h"

class Test : public Counter<Test>
{
public:
	void Display() const
	{
		std::cout << "I am alive ! " << this->Id() << std::endl;
	}

	Test()
	{
		std::cout << "Default ctor - " << this->Id() << std::endl; 
	}

	Test(const Test& other)
	{
		std::cout << "Copy ctor from " << other.Id() << " to " << this->Id() << std::endl;
	}

	Test(Test&& other)
	{
		std::cout << "Move ctor from " << other.Id() << " to " << this->Id() << std::endl; 
	}

	Test& operator=(const Test& other)
	{
		std::cout << "Move assignement from " << other.Id() << " to " << this->Id() << std::endl;

		return *this; 
	}
};

void Display(const Test& a)
{
	a.Display();
}

Test CreateInstance()
{
	std::cout << "------ CreateInstance ------" << std::endl;

	Test test;

	return test;
}

template<typename F>
class Func;

template<typename Ret, typename... Args>
class Func<Ret(Args...)>
{
private:
	typedef Ret(*FuncPtr)(Args...);

	FuncPtr f;

public:
	Func(FuncPtr _f) :
		f(_f)
	{}

	Ret operator()(Args ...args) const
	{
		//return (*f)(args...);
		return (*f)(std::forward<Args>(args)...);
	}
};

template<class C, class M>
class MemFunc;

template<class C, class Ret, class... Args>
class MemFunc<C, Ret(Args...)>
{
private:
	typedef Ret(C::*MemFuncPtr)(Args...);

	C* o;
	MemFuncPtr m;

public:
	MemFunc(C* _o, MemFunc _m) :
		o(_o),
		m(_m)
	{}

	Ret operator()(Args ...args)
	{
		return (o.*m)(args...);
	}
};

int main()
{
	/*
	void (Test::*methodptr)() = &Test::Method;

	Test test;

	(test.*methodptr)();

	Func<void(const Test&)> f(Display);

	Test test;

	f(test);
	f(Test());

	std::function<void(const Test&)> m(&Test::Display);

	m(test);

	std::function<void(void)> r = std::bind(&Test::Display, std::cref(test));

	r();
	*/

	std::packaged_task<Test(void)> pt(CreateInstance);

	std::cout << "----------------------" << std::endl;

	pt();

	std::cout << "----------------------" << std::endl;

	Test ret = pt.get_future().get();

	/*
	Test test;

	MemFunc<void(const Test&)> mem = &Test::Display;
	*/

	//MemFunc<Test, void(void)> m(&test, &Test::Method);

	std::cin.get();
}