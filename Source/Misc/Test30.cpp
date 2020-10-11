#include <iostream>
#include <functional>
#include <vector>
#include <string>

#include "Utilities/Source/InstanceCounter.h"

class Observable
{
	typedef std::function<void()> Signal;

public:
	void AddListener(const Signal& f)
	{
		m_Listeners.emplace_back(f);
	}

	void AddListener(Signal&& f)
	{
		m_Listeners.emplace_back(std::move(f));
	}

	void Emmit() const
	{
		for (auto& f : m_Listeners)
			f();
	}

private:
	std::vector<Signal> m_Listeners;
};

void f()
{
	std::cout << "Hello world !" << std::endl;
}

class Test : public Counter<Test>
{
public:
	Test(const char* name) : m_Name(name) {}
	Test(const Test&) : Counter<Test>()	{}

	void Hello() const
	{
		std::cout << "Hello world from Test class ! " << m_Name << ' ' << this->Id() << std::endl;
	}

private:
	std::string m_Name;
};

int main()
{
	{
		Observable obs;
		Test test("Matt");

		obs.AddListener(f);
		obs.AddListener(std::bind(&Test::Hello, std::cref(test)));

		{
			Test test2("Don");
			obs.AddListener(std::bind(&Test::Hello, std::cref(test2)));
		}

		int a = 0;
		int b = 0;
		int c = 0;
		int d = 0;
		int e = 0;
		int f = 0;

		obs.Emmit();
	}

	std::cin.get();
}