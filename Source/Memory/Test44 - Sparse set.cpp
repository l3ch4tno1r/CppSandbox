#include <iostream>

#include "Containers/SparseSet.h"
#include "Utilities/Source/InstanceCounter.h"

class TestComponent : public Counter<TestComponent>
{
public:
	TestComponent()                           { std::cout << '#' << this->Id() << " default ctor"    << std::endl; }
	//TestComponent(const TestComponent& other) { std::cout << '#' << this->Id() << " copy ctor from " << other.Id() << std::endl; }
	TestComponent(const TestComponent&) = delete;
	TestComponent(TestComponent&& other)      { std::cout << '#' << this->Id() << " move ctor from " << other.Id() << std::endl; }
	~TestComponent()                          { std::cout << '#' << this->Id() << " dtor"            << std::endl; }

	TestComponent& operator=(const TestComponent& other) { std::cout << '#' << this->Id() << " copy assign from " << other.Id() << std::endl; return *this; }
	TestComponent& operator=(TestComponent&& other)      { std::cout << '#' << this->Id() << " move assign from " << other.Id() << std::endl; return *this; }

	void Display() const { std::cout << "Hello world ! #" << this->Id() << std::endl; }
};

int main()
{
	try
	{
		LCN::SparseSet<TestComponent> set;

		LCN::Entity entt[5] = { 5, 3, 0, 1, 8 };

		for (LCN::Entity entity : entt)
			set.Emplace(entity);

		for (const TestComponent& cmp : set)
			cmp.Display();

		std::cout << "---------------" << std::endl;

		set.RemoveComponent(entt[2]);

		std::cout << "---------------" << std::endl;

		for (const TestComponent& cmp : set)
			cmp.Display();
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}