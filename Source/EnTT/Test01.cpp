#include <iostream>

#include <External/entt/include/entt.hpp>
#include <LCN_Math/Source/Geometry/Geometry.h>
#include <Utilities/Source/InstanceCounter.h>

#define SEPARATOR(X) std::cout << "---------- " << X << " ----------" << std::endl;

template<typename Type>
class TypeIndexer;

class TypeAutoInc
{
private:
	static size_t Next()
	{
		static size_t i = 0;
		return i++;
	}

	template<typename Type>
	friend class TypeIndexer;
};

template<typename Type>
class TypeIndexer
{
public:
	static size_t Index()
	{
		static size_t idx = TypeAutoInc::Next();
		return idx;
	}
};

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
	SEPARATOR(1)
	{
		entt::registry registry;

		const entt::entity entity = registry.create();
		registry.emplace<LCN::Transform2Df>(entity);

		const auto& t = registry.get<LCN::Transform2Df>(entity);

		auto view = registry.view<LCN::Transform2Df>();

		std::cout << view.get<LCN::Transform2Df>(entity) << std::endl;
	}

	SEPARATOR(2)
	{
		std::cout << TypeIndexer<int>::Index()               << std::endl;
		std::cout << TypeIndexer<float>::Index()             << std::endl;
		std::cout << TypeIndexer<int>::Index()               << std::endl;
		std::cout << TypeIndexer<LCN::Transform2Df>::Index() << std::endl;
	}

	SEPARATOR(3)
	{
		entt::registry registry;

		const entt::entity entity1 = registry.create();
		const entt::entity entity2 = registry.create();
		const entt::entity entity3 = registry.create();

		registry.emplace<int>(entity1, 1);
		registry.emplace<TestComponent>(entity2);
		registry.emplace<int>(entity3, 3);
		registry.emplace<float>(entity3, 4.0f);

		/*
		registry.view<int, float>().each([](int a, float b) 
			{
				std::cout << a << ", " << b << std::endl;
			});
		*/

		//auto& a = registry.get<TestComponent>(entity2);

		registry.destroy(entity2);

		SEPARATOR("Test");
	}

	SEPARATOR(4)
	{
		entt::registry registry;

		const entt::entity entt1 = registry.create();
		const entt::entity entt2 = registry.create();

		SEPARATOR("Add TestComponent to entt1");
		registry.emplace<TestComponent>(entt1);
		SEPARATOR("Add TestComponent to entt2");
		registry.emplace<TestComponent>(entt2);
		SEPARATOR("Display");
		registry.view<TestComponent>().each([](const TestComponent& cmp)
			{
				cmp.Display();
			});
		SEPARATOR("Done");
	}

	SEPARATOR(5)
	{
		entt::registry reg;

		entt::entity e0 = reg.create();
		entt::entity e1 = reg.create();
		entt::entity e2 = reg.create();

		reg.emplace<int>(e0, 1);
		reg.emplace<float>(e1, 2.0f);
		reg.emplace<int>(e2, 3);
		reg.emplace<float>(e2, 4.0f);

		auto lambda = [](entt::entity e, int a)
		{
			std::cout << entt::to_integral(e) << " - " << a << std::endl;
		};

		SEPARATOR("Test 1");

		reg.view<int>().each(lambda);

		SEPARATOR("Test 2");

		auto group = reg.group<int>(entt::exclude<float>);

		group.each(lambda);
	}

	std::cin.get();
}