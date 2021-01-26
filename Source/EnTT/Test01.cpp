#include <iostream>

#include <External/entt/include/entt.hpp>
#include <LCN_Math/Source/Geometry/Geometry.h>

int main()
{
	entt::registry registry;

	const auto entity = registry.create();
	registry.emplace<LCN::Transform2Df>(entity);
	registry.emplace<int>(entity, 12);

	auto view = registry.view<LCN::Transform2Df, int>();

	std::cout << view.get<LCN::Transform2Df>(entity) << std::endl;
	std::cout << view.get<int>(entity) << std::endl;

	std::cin.get();
}