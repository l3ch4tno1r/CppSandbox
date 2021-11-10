#include <iostream>

template<class C>
concept ForCRTP = requires(C c)
{
	c.Print();
};

template<class DerivedType>
class CRTPBase
{
public:
	inline void Print() const { this->Derived().Print(); }

private:
	const DerivedType& Derived() const { return static_cast<const DerivedType&>(*this); }
	      DerivedType& Derived()       { return static_cast<DerivedType&>(*this); }
};

class Test : public CRTPBase<Test>
{
public:
	inline void Print() const { std::cout << "Hello world !" << std::endl; }
};

int main()
{
	Test test;

	test.Print();

	CRTPBase<Test>& ref = test;

	ref.Print();

	std::cin.get();
}