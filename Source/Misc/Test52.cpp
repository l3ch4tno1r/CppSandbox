#include <vector>
#include <algorithm>

#include "Utilities/Source/Noisy.h"

void Function();

template<typename F>
void Function(F f)
{
	f();
}

void Function()
{
	Function([]() { std::cout << "Default" << std::endl; });
}

int main()
{
	{
		Noisy noisy;

		std::cout << "--------------------" << std::endl;

		auto l1 = [noisy]() {};

		std::cout << "--------------------" << std::endl;

		auto l2 = [&noisy]() {};

		std::cout << "--------------------" << std::endl;

		class
		{
		public:
			void operator()(uint8_t count)
			{
				if (count == 0)
					return;

				std::cout << "Recursive " << (int)count << std::endl;

				(*this)(count - 1);
			}
		} recursive;

		recursive(3);

		std::cout << "--------------------" << std::endl;

		std::vector<int> vec = { -1, 0, 2, 1, 8, -3 };

		class
		{
		public:
			void operator()(int a)
			{
				std::cout << "Print " << a << std::endl;
			}
		}
		print;

		std::for_each(vec.begin(), vec.end(), print);

		std::cout << "--------------------" << std::endl;

		std::for_each(vec.begin(), vec.end(), [=](int a)
		{
			noisy;

			std::cout << a << std::endl;
		});

		std::cout << "--------------------" << std::endl;

		Function();
		Function([]() { std::cout << "Test" << std::endl; });
	}

	std::cin.get();
}