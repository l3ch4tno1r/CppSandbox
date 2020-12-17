#include <iostream>

#include <Utilities/Source/ConfigManager.h>

int main()
{
	try
	{
		auto& appsettings = LCNUtilities::ConfigManager::AppSettings();

		char key = appsettings["goforward"];

		std::cout << key << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}