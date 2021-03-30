#include <iostream>

#include "External/entt/include/entt.hpp"

int main()
{
	entt::registry lregistry;

	entt::entity entities[5];

	lregistry.create(entities, entities + 5);

	lregistry.emplace<int>(entities[0], 1);
	lregistry.emplace<int>(entities[2], 2);
	lregistry.emplace<int>(entities[3], 3);
	lregistry.emplace<int>(entities[4], 4);

	lregistry.emplace<float>(entities[2], 1.0f);

	lregistry.remove<int>(entities[2]);

	auto view = lregistry.view<int, float>();

	for (auto entity : view)
	{
		auto [i, f] = lregistry.get<int, float>(entity);

		std::cout << i << ", " << f << std::endl;
	}

	entt::type_list<int, float>;

	std::cin.get();
}