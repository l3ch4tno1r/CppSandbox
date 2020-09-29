#include <iostream>

void Print()
{
	std::cout << "Hello world !" << std::endl;
}

class Printer
{
public:
	void operator()()
	{
		std::cout << "Hello world from Printer !" << std::endl;
	}
};

template<typename F>
void Test(F f)
{
	f();
}

int main()
{
	Test(Print);
	Test(Printer());
	//Test(1);

	std::cin.get();
}