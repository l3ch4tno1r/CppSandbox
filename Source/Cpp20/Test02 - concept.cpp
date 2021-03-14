#include <iostream>

template<class C>
concept HasPrintMethod = requires(C c)
{
	{ c.Print() };
};

template<class Object> requires
	HasPrintMethod<Object>
class Traits
{
public:
	enum
	{
		HasPrintMethod = HasPrintMethod<Object>
	};
};

template<class E>
class PrintBase
{
	//static_assert(Traits<E>::HasPrintMethod, "No Print method.");

public:
	void Print() { return this->Object().Print(); }

protected:
	      E& Object()       { return static_cast<E&>(*this); }
	const E& Object() const { return static_cast<const E&>(*this); }
};

//----------------------//

class Test : public PrintBase<Test>
{
public:
	using Base = PrintBase<Test>;

	void Print() { std::cout << " Hello world !" << std::endl; }
};

//----------------------//

int main()
{
	Test test;

	test.Print();

	std::cout << Traits<Test>::HasPrintMethod << ", " << HasPrintMethod<Test> << std::endl;

	std::cin.get();
}