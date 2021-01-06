#include <iostream>

#include <Utilities/Source/InstanceCounter.h>

class Test : public Counter<Test>
{
public:
	Test()            { std::cout << this->Id() << " - Default ctor" << std::endl; }
	Test(const Test&) { std::cout << this->Id() << " - Copy ctor" << std::endl; }
	Test(Test&&)      { std::cout << this->Id() << " - Move ctor" << std::endl; }
	~Test()           { std::cout << this->Id() << " - dtor" << std::endl; }
};

template<typename...Args>
void Function(Args... args)
{
	Print(std::forward<Args>(args)...);
}

void Print(const Test& test)
{
	std::cout << "Print lvalue Test #" << test.Id() << std::endl;
}

void Print(Test&& test)
{
	std::cout << "Print rvalue Test #" << test.Id() << std::endl;
}

int main()
{
	{
		Test test;

		Function(std::cref(test));
		Function(Test());
	}

	std::cin.get();
}