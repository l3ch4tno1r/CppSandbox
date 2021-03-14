#include <iostream>

#include "External/entt/include/entt.hpp"

#define SEPARATOR(X) std::cout << "---------- " << X << " ----------" << std::endl;

int main()
{
	SEPARATOR(1);
	{
		entt::registry registry;

		entt::entity entity1 = registry.create();
		entt::entity entity2 = registry.create();
		entt::entity entity3 = registry.create();

		registry.emplace<int>(entity1, entt::to_integral(entity1));
		registry.emplace<int>(entity2, entt::to_integral(entity2));
		registry.emplace<int>(entity3, entt::to_integral(entity3));

		auto lambda = [](entt::entity entity, const int& cmp)
		{
			std::cout << entt::to_integral(entity) << " - " << &cmp << std::endl;
		};

		auto view = registry.view<int>();

		view.each(lambda);

		SEPARATOR("Test");

		registry.remove<int>(entity2);

		std::cout << entt::type_index<int>::value << std::endl;
		std::cout << entt::type_index<float>::value << std::endl;

		view.each(lambda);
	}

	std::cin.get();
}