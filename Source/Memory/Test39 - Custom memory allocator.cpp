#include <iostream>
#include <memory>
#include <thread>

int main()
{
	{
		std::unique_ptr<int> ptr = std::make_unique<int>();

		*ptr = 12;

		std::cout << *ptr << std::endl;
	}

	try
	{
		int* ptr = new int[0];
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}