#include <iostream>

template<size_t I>
class Test
{
public:
	void Print() const;
};

template<>
void Test<0>::Print() const
{
	std::cout << "Hello world !" << std::endl;
}

int main()
{
	Test<0> test;

	test.Print();

	Test<0>().Print();

	std::cin.get();
}
