#include <iostream>
#include <bitset>
#include <cstdlib>
#include <vector>
#include <memory>
#include <type_traits>

#include "External/entt/include/entt.hpp"
#include "Utilities/Source/TimeMeasurement.h"

struct Base
{
	virtual void Display() const = 0;	
	virtual void Increment() = 0;	
};

using BaseUPtr = std::unique_ptr<Base>;
using BasePtr  = Base*;

struct A : Base
{
	int a;

	A(int _a) : a(_a) {}

	void Display() const override { std::cout << "Type A : " << a << std::endl; }
	void Increment() override { a++; }
};

struct B : Base
{
	int b;

	B(int _b) : b(_b) {}

	void Display() const override { std::cout << "Type B : " << b << std::endl; }
	void Increment() override { b++; }
};

struct C : Base
{
	int c;

	C(int _c) : c(_c) {}

	void Display() const override { std::cout << "Type C : " << c << std::endl; }
	void Increment() override { c++; }
};

int main()
{
	const int size = 1000000;

	std::vector<BaseUPtr> _oop; _oop.reserve(size);
	std::vector<BasePtr>  _dop; _dop.reserve(size);

	std::vector<A> _a; _a.reserve(size);
	std::vector<B> _b; _b.reserve(size);
	std::vector<C> _c; _c.reserve(size);

	for (int i = 0; i < size; ++i)
	{
		int r = std::rand() % 3;
		//int r = 0;

		switch (r)
		{
		case 0:
			_oop.emplace_back(std::make_unique<A>(0));

			_a.emplace_back(0);
			_dop.emplace_back(&_a.back());

			break;
		case 1:
			_oop.emplace_back(std::make_unique<B>(0));

			_b.emplace_back(0);
			_dop.emplace_back(&_b.back());

			break;
		case 2:
			_oop.emplace_back(std::make_unique<C>(0));

			_c.emplace_back(0);
			_dop.emplace_back(&_c.back());

			break;
		}
	}

	{
		Timer timer;

		for (auto& ptr : _oop)
			ptr->Increment();
	}

	std::cout << "----------------------" << std::endl;

	{
		Timer timer;

		for (auto ptr : _dop)
			ptr->Increment();
	}

	std::cout << "----------------------" << std::endl;

	{
		Timer timer;

		for (A& a : _a)
			a.Increment();

		for (B& b : _b)
			b.Increment();

		for (C& c : _c)
			c.Increment();
	}

	std::cout << "End" << std::endl;

	std::cin.get();
}