#include <iostream>
#include <utility>

class TestStruct
{
public:
	TestStruct(int _a, int _b) : a(_a), b(_b) {}

	operator std::pair<int, int>() const
	{
		return { a, b };
	}

private:
	int a, b;
};

auto Test()
{
	struct
	{
		int a, b;
	}
	result{ 1, 2 };

	return result;
}

int main()
{
	auto [x, y] = Test();

	std::cout << x << ' ' << y << std::endl;

	TestStruct test{ 3, 4 };

	auto [z, t] = (std::pair<int, int>)test;

	std::cout << z << ' ' << t << std::endl;

	std::cin.get();
}