#include <iostream>
#include <string>

#include "Utilities/InstanceCounter.h"
#include "Utilities/Design Patterns/Observer.h"

#define SEPARATOR(X) std::cout << "------------- " << X << " -------------" << std::endl

class Observer : public ObserverBase<Observer>
{
public:
	virtual void Update(const char* msg) = 0;
};

class PaceMaker : public Subject<Observer>
{};

int main()
{
	{
		SEPARATOR("Test 01");

		Subject<Observer> subject;

		Test obs1("Riri");
		Test obs2("Fifi");
		Test obs3("Loulou");

		subject.AddObserver(obs1);
		subject.AddObserver(obs2);
		subject.AddObserver(obs3);

		subject.Notify("Test 01");
	}

	{
		SEPARATOR("Test 02");

		Test obs1("Riri");
		Test obs2("Fifi");
		Test obs3("Loulou");

		Subject<Observer> subject;

		subject.AddObserver(obs1);
		subject.AddObserver(obs2);
		subject.AddObserver(obs3);

		subject.Notify("Test 02");
	}

	{
		SEPARATOR("Test 03");

		Test obs1("Riri");
		Test obs2("Fifi");
		Test obs3("Loulou");

		Subject<Observer> subject;

		subject.AddObserver(obs1);
		subject.AddObserver(obs2);
		subject.AddObserver(obs3);

		{
			Test obs4("Donald");

			subject.AddObserver(obs4);
			subject.AddObserver(obs3);

			subject.Notify("Test 03");
		}

		SEPARATOR("Test 04");

		subject.Notify("Test 04");

		SEPARATOR("Test 05");

		subject.RemoveObserver(obs3);

		subject.Notify("Test 05");
	}

	std::cin.get();
}