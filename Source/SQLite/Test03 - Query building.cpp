#include <iostream>

template<typename T>
class Property
{
public:
	template<typename...Args>
	Property(Args&&...args) :
		m_Data(std::forward<Args>(args)...)
	{}

	operator T&() { return m_Data; }
	operator const T&() const { return m_Data; }

private:
	T m_Data;
};

class Test
{
public:
	const char* Print() const { return m_Name; }

private:
	const char* m_Name = "Jean RAOUL";
};

std::ostream& operator<<(std::ostream& stream, const Test& data)
{
	stream << data.Print();

	return stream;
}

int main()
{
	Property<Test> prop;

	std::cout << prop << std::endl;

	std::cin.get();
}