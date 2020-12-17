#include <iostream>

#include <Utilities/Source/ConfigManager.h>
#include <Utilities/Source/InstanceCounter.h>

class Test : public Counter<Test>
{
public:
	Test() { std::cout << "default ctor : " << this->Id() << std::endl; }

	Test(const Test& other) :
		c(other.c)
	{
		std::cout << "copy ctor : " << this->Id() << std::endl;
	}

	Test(Test&& other) :
		c(other.c)
	{
		std::cout << "move ctor : " << this->Id() << std::endl;
	}

	char C() const { return c; }
	char& C() { return c; }

private:
	char c;
};

std::istream& operator>>(std::istream& istream, Test& arg)
{
	istream >> arg.C();

	return istream;
}

int main()
{
	try
	{
		auto& appsettings = LCNUtilities::ConfigManager::AppSettings();

		Test key1 = appsettings["goforward"];
		Test key2 = appsettings["gobackward"];
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}