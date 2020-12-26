#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "Utilities/Source/InstanceCounter.h"
#include "Utilities/Source/ErrorHandling.h"

#include "Containers/DynamicArray.h"

#include "Algorithms/Algorithms.h"

#define SEPARATOR(X) std::cout << "\n----------- " << X << " -----------" << std::endl

class Test : public Counter<Test>
{
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

private:
	std::string m_Name;
};

int main()
{
	{
		/*
		*/
		SEPARATOR("Dynamic array tests");
		{
			LCN::DynamicArray<Test> vec;

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

			SEPARATOR("RandomIterator 1");

			for(auto it = vec.Begin(); it != vec.End(); ++it)
				std::cout << "RandomIterator test : " << it->Name() << std::endl;

			SEPARATOR("RandomIterator 2");

			auto it1 = vec.Begin();
			auto it2 = it1++;

			std::cout << "RandomIterator test : " << it1->Name() << std::endl;
			std::cout << "RandomIterator test : " << it2->Name() << std::endl;
			std::cout << "RandomIterator test : " << (it1 + 2)->Name() << std::endl;
			std::cout << "RandomIterator test : " << (vec.End() - 1)->Name() << std::endl;

			SEPARATOR("Erase 1");

			for (auto it = vec.Begin(); it != vec.End(); ++it)
				std::cout << "Before erase : " << it->Name() << std::endl;

			vec.Erase(vec.Begin() + 2);

			for (auto it = vec.Begin(); it != vec.End(); ++it)
				std::cout << "After erase : " << it->Name() << std::endl;

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

			SEPARATOR("Erase 2");

			vec.Erase(vec.Begin() + 1, vec.Begin() + 3);

			for (auto it = vec.Begin(); it != vec.End(); ++it)
				std::cout << "After erase 2 : " << it->Name() << std::endl;

			SEPARATOR("RandomIterator 3");

			std::cout << vec.Begin()[0].Name() << std::endl;
			std::cout << vec.Begin()[1].Name() << std::endl;
			std::cout << vec.Begin()[2].Name() << std::endl;
		}

		// Algorithm tests
		/*
		{
			SEPARATOR("Algorithms");

			std::vector<int> temp = { 91, 27, 18, 53, 46, 86, 79, 6, 17, 8, 73, 16, 56, 55, 14 };

			std::cout << "Is heap : " << std::is_heap(temp.begin(), temp.end()) << std::endl;;

			LCN::DynamicArray<int> vec2 = { 91, 27, 18, 53, 46, 86, 79, 6, 17, 8, 73, 16, 56, 55, 14 }; //, 6, 97, 74, 20, 40, 13, 47, 4, 67, 99, 3, 83, 57, 89, 51 };

			LCN::MakeHeap(vec2.Begin(), vec2.End());

			auto it5 = temp.begin();
			auto it6 = vec2.Begin();

			while (it6 != vec2.End())
				*(it5++) = *(it6++);

			std::cout << "Is heap : " << std::is_heap(temp.begin(), temp.end()) << std::endl;;

			for (int e : temp)
				std::cout << e << std::endl;
		}
		*/

		SEPARATOR("End");
	}

	std::cin.get();
}