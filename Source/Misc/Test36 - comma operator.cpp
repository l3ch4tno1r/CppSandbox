#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <tuple>

using namespace std::literals::string_literals;

template<typename T>
class CommaInitializer
{
public:
	CommaInitializer(std::vector<T>& vec, const T& val) :
		m_Target(vec)
	{
		m_Target.push_back(val);
	}

	CommaInitializer& operator,(const T& val)
	{
		m_Target.push_back(val);

		return *this;
	}

private:
	std::vector<T>& m_Target;
};

template<typename T>
CommaInitializer<T> operator<<(std::vector<T>& vec, const T& val)
{
	return CommaInitializer<T>(vec, val);
}

class Test
{
public:
	Test(const std::vector<int>& vec) :
		m_Vec(vec)
	{}

	Test(std::vector<int>&& vec) :
		m_Vec(std::move(vec))
	{}

private:
	std::vector<int> m_Vec;

	friend Test& operator,(Test&, int);
	friend void Print(const Test&);
};

Test& operator,(Test& a, int b)
{
	a.m_Vec.push_back(b);

	return a;
}

void Print(const Test& test)
{
	for (int i : test.m_Vec)
		std::cout << i << ' ';

	std::cout << std::endl;
}

void Print()
{}

template<typename...Args>
void Print(int first, Args... args)
{
	std::cout << first << std::endl;
	Print(args...);
}

template<typename...Args>
std::array<std::string, sizeof...(Args)> CreateArray(Args... args)
{
	std::array<std::string, sizeof...(Args)> result = { args... };

	return result;
}

template<typename...Args>
std::tuple<Args...> CreateTuple(Args... args)
{
	return std::tuple<Args...>(args...);
}


int main()
{
	std::vector<int> vec;

	vec << 2, 3, 4, 5;

	Test test(std::move(vec));

	test, 1, 2, 3;

	Print((test, 8));

	Print(1, 2, 3, 4, 1.5f);

	auto arr = CreateArray("Joe", "Jack");

	std::tuple<int, int, std::string> tuple = CreateTuple(1, 2, "Joe");

	std::cout << std::get<2>(tuple) << std::endl;

	std::cin.get();
}