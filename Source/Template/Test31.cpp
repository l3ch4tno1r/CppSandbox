#include <iostream>
#include <functional>
#include <list>
#include <utility>
#include <string>

#include <functional>
#include <type_traits>

#include "Utilities/InstanceCounter.h"

#pragma region Seen on stackoverflow
template<int...>
struct int_sequence
{};

template<int N, int... Is>
struct make_int_sequence : make_int_sequence<N - 1, N - 1, Is...>
{};

template<int... Is>
struct make_int_sequence<0, Is...> : int_sequence<Is...>
{};

template<int> // begin with 0 here!
struct placeholder_template
{};

namespace std
{
	template<int N>
	struct is_placeholder<placeholder_template<N>> : integral_constant<int, N + 1> // the one is important
	{};
}
#pragma endregion

template<class C, typename ...Args>
class ISubject
{
private:
	std::list<std::pair<C*, std::function<void(Args...)>>> m_Observers;

	template<int...Is>
	void AddObserver(C& observer, int_sequence<Is...>)
	{
		std::function<void(Args...)> temp = std::bind(&C::Update, std::ref(observer), placeholder_template<Is>{}...);

		std::pair<C*, std::function<void(Args...)>> item(&observer, std::move(temp));
		m_Observers.push_back(item);
	}

public:
	void AddObserver(C& observer)
	{
		AddObserver(observer, make_int_sequence<sizeof...(Args)>{});
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

		std::cout << '#' << this->Id() << " -> " << m_Name << std::endl;
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

int main()
{
	ISubject<IObserver, const char*> subject;
	IObserver obs1;
	IObserver obs2;
	IObserver obs3;

	subject.AddObserver(obs1);
	subject.AddObserver(obs2);
	subject.AddObserver(obs3);

	subject.Notify("Hello world !");

	/*
	IObserver obs1;

	std::function<void(const char*)> f = std::bind(&IObserver::Update, std::ref(obs1), std::placeholders::_1);

	f("Matt");

	std::cout << "Final name : "<< obs1.Name() << std::endl;

	gen_seq<4> test;
	*/

	std::cin.get();
}