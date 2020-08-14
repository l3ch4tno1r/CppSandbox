#include <iostream>
#include <functional>

#include "Utilities/InstanceCounter.h"

void f()
{
	std::cout << "Hello world" << std::endl;
}

struct Test : public Counter<Test>
{
	void Display() const
	{
		std::cout << "I am alive ! " << this->Id() << std::endl;
	}

	Test()            { std::cout << "Hello world ! - default " << this->Id() << std::endl; }
	Test(const Test&) { std::cout << "Hello world ! - copy "    << this->Id() << std::endl; }
	Test(Test&&)      { std::cout << "Hello world ! - moved "   << this->Id() << std::endl; }
};

int main()
{
	std::function<void()> func = f;

	func();

	Test test;

	std::function<void(const Test&)> mem = &(Test::Display);

	mem(test);
	mem(Test());

	std::cin.get();
}