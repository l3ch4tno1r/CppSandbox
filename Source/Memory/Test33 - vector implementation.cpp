#include <iostream>
#include <string>
#include <algorithm>

#include "Utilities/InstanceCounter.h"
#include "Utilities/ErrorHandling.h"

#include "Containers/DynamicArray.h"

#define SEPARATOR(X) std::cout << "\n----------- " << X << " -----------" << std::endl

class Test : public Counter<Test>
{
private:
	std::string m_Name;

public:
	Test() :
		m_Name("No name")
	{
		std::cout << "#" << this->Id() << " Hello world " << m_Name << std::endl;
	}

	Test(const char* str) :
		m_Name(str)
	{
		std::cout << "#" << this->Id() << " Hello world " << m_Name << std::endl;
	}

	Test(const Test& other) :
		m_Name(other.m_Name)
	{
		std::cout << "#" << this->Id() << " Hello world - Copy " << m_Name << std::endl;
	}

	Test(Test&& other) :
		m_Name(std::move(other.m_Name))
	{
		std::cout << "#" << this->Id() << " Hello world - Move " << m_Name << std::endl;
	}

	~Test()
	{
		std::cout << "#" << this->Id() << " Goodbye world " << m_Name << std::endl;
	}

	Test& operator=(const Test& other)
	{
		std::cout << "#" << this->Id() << " copy assigned " << m_Name << std::endl;

		m_Name = other.m_Name;

		return *this;
	}

	Test& operator=(Test&& other)
	{
		m_Name = std::move(other.m_Name);

		std::cout << "#" << this->Id() << " move assigned " << this->Name() << std::endl;

		return *this;
	}

	const std::string& Name() const
	{
		return m_Name;
	}
};

int main()
{
	{
		DynamicArray<Test> vec;

		SEPARATOR("Add Joe");
		vec.EmplaceBack("Joe");
		SEPARATOR("Add Jack");
		vec.EmplaceBack("Jack");
		SEPARATOR("Add William");
		vec.EmplaceBack("William");
		SEPARATOR("Add Averell");
		vec.EmplaceBack("Averell");
		SEPARATOR("Add Matt");
		vec.EmplaceBack("Matt");

		SEPARATOR("Display");

		for (size_t i = 0; i < vec.Size(); ++i)
			std::cout << vec[i].Id() << " - " << vec[i].Name() << std::endl;

		SEPARATOR("Pop");

		vec.PopBack();

		for (size_t i = 0; i < vec.Size(); ++i)
			std::cout << vec[i].Id() << " - " << vec[i].Name() << std::endl;

		SEPARATOR("Clear");

		vec.Clear();

		vec.EmplaceBack("Joe");
		vec.EmplaceBack("Jack");
		vec.EmplaceBack("William");
		vec.EmplaceBack("Averell");
		vec.EmplaceBack("Matt");

		SEPARATOR("Iterator 1");

		for(auto it = vec.Begin(); it != vec.End(); ++it)
			std::cout << "Iterator test : " << it->Name() << std::endl;

		SEPARATOR("Iterator 2");

		auto it1 = vec.Begin();
		auto it2 = it1++;

		std::cout << "Iterator test : " << it1->Name() << std::endl;
		std::cout << "Iterator test : " << it2->Name() << std::endl;
		std::cout << "Iterator test : " << (it1 + 2)->Name() << std::endl;
		std::cout << "Iterator test : " << (vec.End() - 1)->Name() << std::endl;

		SEPARATOR("Erase 1");

		for (auto it = vec.Begin(); it != vec.End(); ++it)
			std::cout << "Before erase : " << it->Name() << std::endl;

		vec.Erase(vec.Begin() + 2);

		for (auto it = vec.Begin(); it != vec.End(); ++it)
			std::cout << "After erase : " << it->Name() << std::endl;
		/*
		*/

		SEPARATOR("Insert");

		vec.Insert(vec.End(), "Melody");

		for (auto it = vec.Begin(); it != vec.End(); ++it)
			std::cout << "Before insert : " << it->Name() << std::endl;

		vec.Insert(vec.Begin() + 4, {
			Test("Bob"),
			Test("Gratt"),
			Test("Bill"),
			Test("Emmett")
		});

		for (auto it = vec.Begin(); it != vec.End(); ++it)
			std::cout << "After insert : " << it->Name() << std::endl;

		/*
		*/
		SEPARATOR("Erase 2");

		vec.Erase(vec.Begin() + 1, vec.Begin() + 3);

		for (auto it = vec.Begin(); it != vec.End(); ++it)
			std::cout << "After erase 2 : " << it->Name() << std::endl;

		SEPARATOR("End");
	}

	std::cin.get();
}