#include <iostream>
#include <type_traits>

int main()
{
	int a = 1;

	struct {
		int x;
		float y;
	} b{ 2, 3.0f };

	auto tset1 = [=]()
	{
		return a + 1;
	};

	auto tset2 = [&]()
	{
		b.x = 0;

		return a + 1;
	};

	auto tset3 = [&]()
	{
		return b.y;
	};

	auto tset4 = [=]()
	{
		return a + b.x;
	};

	std::cout << sizeof(tset1) << std::endl;
	std::cout << sizeof(tset2) << std::endl;
	std::cout << sizeof(tset3) << std::endl;
	std::cout << sizeof(tset4) << std::endl;

	std::cout << std::boolalpha << std::is_same<decltype(tset1), decltype(tset2)>::value << std::endl;

	std::cin.get();
}