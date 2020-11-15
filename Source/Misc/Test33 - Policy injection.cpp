#include <iostream>

template<class ColorPolicy>
class Pen : public ColorPolicy
{
public:
	void Print() const
	{
		std::cout << "This pen is " << ColorPolicy::operator[](0) << std::endl;
		std::cout << "This pen is " << ColorPolicy::operator[](1) << std::endl;
		std::cout << "This pen is " << ColorPolicy::operator[](2) << std::endl;
	}
};

class ColorRed
{
public:
	const char* GetColor() const { return m_Color; }

	char operator[](size_t i) const { return m_Color[i]; }

private:
	const char* m_Color = "red";
};

int main()
{
	Pen<ColorRed> pen;

	pen.Print();

	std::cin.get();
}