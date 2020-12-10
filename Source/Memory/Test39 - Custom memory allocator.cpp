#include <iostream>
#include <memory>
#include <vector>

#include <Utilities/Source/InstanceCounter.h>

#define SEPARATOR(X) std::cout << "----- " << X << " -----" << std::endl;

class Test : public Counter<Test>
{
public:
	Test() { std::cout << "Hello world " << this->Id() << std::endl; }
	~Test() { std::cout << "Goodbye world " << this->Id() << std::endl; }
};

int main()
{
	try
	{
		SEPARATOR(0);
		{
			std::unique_ptr<int> ptr = std::make_unique<int>();

			*ptr = 12;

			std::cout << *ptr << std::endl;
		}

		SEPARATOR(1);
		{
			Test* ptr = new Test[10];

			delete[] ptr;
		}

		SEPARATOR(2);
		{
			std::vector<Test> vector;

			vector.reserve(10);

			for (int i = 0; i < vector.capacity(); i++)
				vector.emplace_back();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}