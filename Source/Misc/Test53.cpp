#include <iostream>

class Test
{
public:
	inline void Action() { m_Action(*this); }

private:
	using FPtr = void (*)(Test&);

	FPtr m_Action{ LazyLoad };

	static void LazyLoad(Test& test) { std::cout << "LazyLoad" << std::endl; test.m_Action = CacheAccess; }
	static void CacheAccess(Test&)   { std::cout << "CacheAccess" << std::endl; }
};

int main()
{
	Test test;

	test.Action();
	test.Action();
	test.Action();
	test.Action();

	using ptr = decltype(Test::Action);

	std::cin.get();
}