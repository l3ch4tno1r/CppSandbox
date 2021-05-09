#include <iostream>

#include <Utilities/Source/InstanceCounter.h>

#define SEPARATOR(X) std::cout << "-------- " << #X << " --------" << std::endl;

class Test : public Counter<Test>
{
public:
	Test()            { std::cout << this->Id() << " - Default ctor" << std::endl; }
	Test(const Test&) { std::cout << this->Id() << " - Copy ctor" << std::endl; }
	Test(Test&&)      { std::cout << this->Id() << " - Move ctor" << std::endl; }
	~Test()           { std::cout << this->Id() << " - dtor" << std::endl; }
};

template<typename TestClass>
void Function(TestClass&& arg)
{
	Print(std::forward<TestClass>(arg));
}

void Print(Test& test)
{
	std::cout << "Print non const lvalue Test #" << test.Id() << std::endl;
}

void Print(const Test& test)
{
	std::cout << "Print const lvalue Test #" << test.Id() << std::endl;
}

void Print(Test&& test)
{
	std::cout << "Print rvalue Test #" << test.Id() << std::endl;
}

int main()
{
	{
		Test test;

		SEPARATOR(1)
		Function(test);

		SEPARATOR(2)
		Function(std::cref(test));

		SEPARATOR(3)
		Function(std::move(test));

		SEPARATOR(4)
		Function(Test());

		SEPARATOR(End)
	}

	std::cin.get();
}