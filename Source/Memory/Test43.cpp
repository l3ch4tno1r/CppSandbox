#include <iostream>
#include <unordered_map>
#include <string>

#include <Utilities/Source/InstanceCounter.h>

class Test
{
public:
	//Test() { std::cout << this->Id() << " - Default ctor" << std::endl; }

	Test(size_t i) :
		m_Id(i)
	{ std::cout << this->Id() << " - Custom ctor" << std::endl; }

	Test(const Test& other) :
		m_Id(other.m_Id)
	{ std::cout << this->Id() << " - Copy ctor from " << other.Id() << std::endl; }

	Test(Test&& other) :
		m_Id(other.m_Id) 
	{ std::cout << this->Id() << " - Move ctor from " << other.Id() << std::endl; }

	~Test() { std::cout << this->Id() << " - dtor" << std::endl; }

	Test& operator=(const Test& other) { std::cout << this->Id() << " - Copy assign from " << other.Id() << std::endl; }
	Test& operator=(Test&& other)      { std::cout << this->Id() << " - Move assign from " << other.Id() << std::endl; }

	friend class std::hash<Test>;

	size_t Id() const { return m_Id; };

private:
	size_t m_Id = 0;
};

template<>
class std::hash<Test>
{
public:
	size_t operator()(const Test& test) const
	{
		return std::hash<uint32_t>()(test.Id());
	}
};

bool operator==(const Test& a, const Test& b) { return a.Id() == b.Id(); }

int main()
{
	std::unordered_map<Test, std::string> map;

	map[0] = "Joe";
	map[1] = "Jack";
	map[2] = "William";
	map[3] = "Averell";

	for (auto it = map.cbegin(); it != map.cend(); ++it)
		std::cout << '#' << it->first.Id() << " - " << it->second << std::endl;

	map.erase(1);

	for (auto it = map.cbegin(); it != map.cend(); ++it)
		std::cout << '#' << it->first.Id() << " - " << it->second << std::endl;

	std::cin.get();
}