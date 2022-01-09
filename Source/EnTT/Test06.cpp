#include <iostream>
#include <functional>

#include "External/entt/include/entt.hpp"
#include "Source/Memory/_MemTracker.h"

#define SEPARATOR(X) std::cout << "--------- " << X << " ---------\n"

struct A
{
	int a;
};

struct B
{
	int b;
};

struct C
{
	int c;
};

int main()
{
	SEPARATOR("Begin");
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		entt::registry _reg;

		SEPARATOR("Fill register");

		for (int i = 0; i < 100; ++i)
		{
			auto entity = _reg.create();

			union
			{
				struct
				{
					bool a : 1;
					bool b : 1;
					bool c : 1;
				};
				char byte;
			} val;

			val.byte = std::hash<int>()(i);

			if (val.a)
				_reg.emplace<A>(entity, i);

			if (val.b)
				_reg.emplace<B>(entity, i);

			if (val.c)
				_reg.emplace<C>(entity, i);
		}

		auto lambda = [](const A& a, const B& b)
		{
			int adra = (int)&a, adrb = (int)&b;

			std::cout
				<< adra << " - " << adrb
				<< " = " << adra - adrb
				<< std::endl;
		};

		SEPARATOR("Create view");

		auto viewAB = _reg.view<A, B>();

		viewAB.each(lambda);

		SEPARATOR("Create group 1");

		auto groupAB = _reg.group<A, B>();

		groupAB.each(lambda);

		SEPARATOR("Create new entity");

		auto entity = _reg.create();

		_reg.emplace<A>(entity, (int)entt::to_integral(entity));
		_reg.emplace<B>(entity, (int)entt::to_integral(entity));

		groupAB.each(lambda);

		SEPARATOR("End");
	}

	std::cin.get();
}