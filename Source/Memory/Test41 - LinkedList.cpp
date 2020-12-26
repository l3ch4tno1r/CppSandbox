#include <iostream>
#include <string>
#include <array>

#include "Containers/StaticArray.h"
#include "Containers/LinkedList.h"

#include "_MemTracker.h"

#include "Utilities/Source/InstanceCounter.h"
#include "Utilities/Source/ErrorHandling.h"

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

#define DISPLAY_ENUMERABLE(X) \
for (auto _it = X.Begin(); _it != X.End(); ++_it)\
	std::cout << "Test - " << _it->Name() << std::endl;

int main()
{
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		LCN::LinkedList<Test> list;

		list.EmplaceBack("Joe");
		list.EmplaceBack("Jack");
		list.EmplaceBack("William");
		list.EmplaceBack("Averell");

		auto it = list.Begin();

		while(it != list.End())
			std::cout << "Test - " << (it++)->Name() << std::endl;

		SEPARATOR('a');

		it = list.Begin();

		std::cout << (it++)->Name() << std::endl;
		std::cout << (--it)->Name() << std::endl;

		SEPARATOR('b');

		list.EmplaceBack(Test("Matt"));

		DISPLAY_ENUMERABLE(list)

		SEPARATOR('c');

		it = list.Begin();

		++it; ++it;

		std::cout << "Inserting before : " << it->Name() << std::endl;

		auto it2 = list.Emplace(it, "Melody");

		DISPLAY_ENUMERABLE(list)

		std::cout << "Current       : " << it2->Name() << std::endl;
		std::cout << "After Melody  : " << (++it2)->Name() << std::endl;
		std::cout << "Before Melody : " << (--(--it2))->Name() << std::endl;

		SEPARATOR('d');

		try
		{
			list.Remove(++it2);
			list.Remove(list.Begin());
			list.Remove(list.End());
		}
		catch (const std::exception& e)
		{
			std::cerr << "Exception : " << e.what() << std::endl;
		}

		DISPLAY_ENUMERABLE(list)

		SEPARATOR('e');

		list.PopBack();

		DISPLAY_ENUMERABLE(list)

		ASSERT(list.Size() == 3);

		SEPARATOR("End of scope");
	}

	SEPARATOR("End");

	std::cin.get();
}