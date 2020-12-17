#include <iostream>
#include <string>

template<class T>
class MetaData;

class Test
{
public:
	const std::string& Name() const { return m_Name; }

private:
	std::string m_Name;
};

template<class T>
class Property
{
public:
	Property()
	{}

private:
	std::string m_Name
};

template<>
class MetaData<Test>
{};

int main()
{


	std::cin.get();
}