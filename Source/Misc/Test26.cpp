#include <iostream>
#include <functional>
#include <list>
#include <utility>
#include <string>

#include "Utilities/InstanceCounter.h"

template<class C, typename ...Args>
class ISubject
{
private:
	std::list<std::pair<C*, std::function<void(Args...)>>> m_Observers;

public:
	void AddObserver(C& observer)
	{
		std::function<void(Args...)> temp = std::bind(&C::Update, std::ref(observer));

		std::pair<C*, std::function<void(Args...)>> item(&observer, std::move(temp));
		m_Observers.push_back(item); 
	}

	void Notify(Args ...args)
	{
		for (auto& e : m_Observers)
			e.second(std::forward<Args>(args)...);
	}
};

class IObserver : public Counter<IObserver>
{
private:
	std::string m_Name;
public:
	IObserver()
	{
		std::cout << "IObserver default ctor #" << this->Id() << std::endl;
	}

	IObserver(const IObserver& other)
	{
		std::cout << "IObserver copy ctor from #" << other.Id() << " to #" << this->Id() << std::endl;
	}

	void Update(const char* str)
	{
		m_Name = str;

		std::cout << m_Name << std::endl;
	}

	IObserver& operator=(const IObserver& other)
	{
		std::cout << "IObserver copy assignement from #" << other.Id() << " to #" << this->Id() << std::endl;
	}

	IObserver& operator=(IObserver&& other)
	{
		std::cout << "IObserver move assignement from #" << other.Id() << " to #" << this->Id() << std::endl;
	}

	const std::string& Name() const { return m_Name; }
};

template <int... Is>
struct index {};

template <int N, int... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

template <int... Is>
struct gen_seq<0, Is...> : index<Is...> {};

int main()
{
	/*
	ISubject<IObserver, const char*> subject;
	IObserver obs1;
	IObserver obs2;
	IObserver obs3;

	subject.AddObserver(obs1);
	subject.AddObserver(obs2);
	subject.AddObserver(obs3);

	subject.Notify("Hello world !");
	*/

	IObserver obs1;

	std::function<void(const char*)> f = std::bind(&IObserver::Update, std::ref(obs1), std::placeholders::_1);

	f("Matt");

	std::cout << "Final name : "<< obs1.Name() << std::endl;

	gen_seq<4> test;

	std::cin.get();
}