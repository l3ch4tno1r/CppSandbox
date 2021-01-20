#include <iostream>

template<class F>
class Functor;

template<class ...Args>
class Functor<void(Args...)>
{
public:
	void operator()(Args...)
	{

	}

private:

};

int main()
{
	std::cin.get();
}