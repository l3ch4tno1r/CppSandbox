#include <iostream>
#include <functional>
#include <list>
#include <utility>
#include <string>

#include <functional>
#include <type_traits>

#include "Utilities/InstanceCounter.h"

#define SEPARATOR(X) std::cout << "------------- " << X << " -------------" << std::endl

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

class IObserver;

template<typename ...Args>
class ISubject
{
private:
	using IObsPtrFctPair = std::pair<IObserver*, std::function<void(Args...)>>;
	std::list<IObsPtrFctPair> m_Observers;

	template<int...Is>
	void AddObserver(IObserver& observer, int_sequence<Is...>)
	{
		std::function<void(Args...)> temp = std::bind(&IObserver::Update, std::ref(observer), placeholder_template<Is>{}...);

		m_Observers.push_back(IObsPtrFctPair(&observer, std::move(temp)));
	}

public:
	void AddObserver(IObserver& observer)
	{
		AddObserver(observer, make_int_sequence<sizeof...(Args)>{});
	}

	void RemoveObserver(IObserver& observer)
	{
		m_Observers.remove_if([&observer](IObsPtrFctPair& elem)
		{
			return elem.first == &observer;
		});
	}

	void Notify(Args ...args)
	{
		for (auto& e : m_Observers)
			e.second(std::forward<Args>(args)...);
	}
};

template<typename Subject>
class IObserverBase
{
private:
	Subject* m_Subject;

public:
	virtual ~IObserverBase()
	{
		m_Subject->RemoveObserver(*this);
	}
};

class IObserver : public Counter<IObserver>, public IObserverBase<ISubject<const char*>>
{
public:
	void Update(const char* str)
	{
		std::cout << "Test : " << this->Id() << ", " << str << std::endl;
	}
};

int main()
{
	ISubject<const char*> subject;
	IObserver obs1;
	IObserver obs2;
	IObserver obs3;

	subject.AddObserver(obs1);
	subject.AddObserver(obs2);
	subject.AddObserver(obs3);

	subject.Notify("Hello world ! 1");

	SEPARATOR(1);

	subject.RemoveObserver(obs2);

	subject.Notify("Hello world ! 2");

	{
		SEPARATOR(2);

		IObserver obs4;

		subject.AddObserver(obs4);
	}

	IObserver obs5;

	subject.AddObserver(obs5);

	subject.Notify("Hello world ! 3");

	/*
	IObserver obs1;

	std::function<void(const char*)> f = std::bind(&IObserver::Update, std::ref(obs1), std::placeholders::_1);

	f("Matt");

	std::cout << "Final name : "<< obs1.Name() << std::endl;

	gen_seq<4> test;
	*/

	std::cin.get();
}