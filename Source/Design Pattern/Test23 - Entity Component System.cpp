#include <iostream>

#include <Utilities/Source/DesignPatterns/EntityComponentSystem.h>

int main()
{
	using Type1 = char;
	using Type2 = short;
	using Type3 = int;

	LCN::EntityManager registry;

	LCN::EntityHandle entity1 = registry.CreateEntity();
	LCN::EntityHandle entity2 = registry.CreateEntity();
	LCN::EntityHandle entity3 = registry.CreateEntity();
	LCN::EntityHandle entity4 = registry.CreateEntity();
	LCN::EntityHandle entity5 = registry.CreateEntity();

	registry.Add<Type1>(entity1, 'a');
	registry.Add<Type2>(entity1, 1);

	registry.Add<Type1>(entity2, 'b');
	registry.Add<Type3>(entity2, 1);

	registry.Add<Type2>(entity3, 2);
	registry.Add<Type3>(entity3, 2);

	registry.Add<Type3>(entity4, 3);

	registry.Add<Type1>(entity5, 'c');
	registry.Add<Type2>(entity5, 3);
	registry.Add<Type3>(entity5, 4);

	std::cout << registry.Get<Type2>(entity3) << std::endl;
	std::cout << registry.Get<Type3>(entity5) << std::endl;
	std::cout << registry.Get<Type3>(entity1) << std::endl;

	std::cin.get();
}