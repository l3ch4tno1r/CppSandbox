#include <iostream>

class Test
{
public:
	int GetINoInline() const;
	inline int GetIInline() const { return i; }
private:
	int i = 12;
};

int main()
{
	Test test;

	int a = test.GetINoInline();
	int b = test.GetIInline();

	std::cin.get();
}

int Test::GetINoInline() const
{
	return i;
}
