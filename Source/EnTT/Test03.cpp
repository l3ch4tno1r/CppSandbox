#include <iostream>

#include "External/entt/include/entt.hpp"
#include "Source/Memory/_MemTracker.h"

#define SEPARATOR(X) std::cout << "---------- " << X << " ----------" << std::endl;

int main()
{
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		SEPARATOR("Create register");
		entt::registry reg;

		SEPARATOR("Create entities");
		entt::entity entity1 = reg.create();
		entt::entity entity2 = reg.create();
		entt::entity entity3 = reg.create();

		SEPARATOR("Add components");
		reg.emplace<int>(entity1, entt::to_integral(entity1));
		reg.emplace<float>(entity1, 1.0f);

		reg.emplace<int>(entity2, entt::to_integral(entity2));

		reg.emplace<int>(entity3, entt::to_integral(entity3));
		reg.emplace<float>(entity3, 3.0f);

		SEPARATOR("Create view");
		auto view = reg.view<int, float>();

		view.each([](int a, float b)
			{
				std::cout << a << ", " << b << std::endl;
			});

		SEPARATOR("Add other component");
		reg.emplace<float>(entity2, 2.0f);

		for (auto it = view.begin(); it != view.end(); ++it)
		{
			int   a = reg.get<int>(*it);
			float b = reg.get<float>(*it);

			std::cout << a << ", " << b << std::endl;
		}

		SEPARATOR("End session");
	}

	std::cin.get();
}