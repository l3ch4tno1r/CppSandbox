#include <iostream>

#include <Utilities/Source/DesignPatterns/Observer.h>

class Test : public Observer<Test>
{
public:
	void Update() { std::cout << "Hello world !" << std::endl; }
};

int main()
{
	{
		Test test;
		Subject<Test> subject;

		subject.AddObserver(test);

		subject.Notify();
	}

	std::cin.get();
}