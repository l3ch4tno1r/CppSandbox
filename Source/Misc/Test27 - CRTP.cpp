#include <iostream>

template<class E>
class Base
{
protected:
	E& Derived() { return static_cast<E&>(*this); }
	const E& Derived() const { return static_cast<const E&>(*this); }
public:
	void PrintBase() const { this->Derived().PrintBase(); }
};

template<class E>
class Derived1 : public Base<E>
{
public:
	void PrintDerived1() const { this->Derived().PrintDerived1(); }
};

class Derived2 : public Derived1<Derived2>
{
public:
	void PrintBase() const { std::cout << "Derived2" << std::endl; }
	void PrintDerived1() const { std::cout << "Derived2 Test" << std::endl; }
};

template<class E>
void Print(const Derived1<E>& a)
{
	a.PrintBase();
	a.PrintDerived1();
}

int main()
{
	Derived2 d2;

	Print(d2);

	std::cin.get();
}