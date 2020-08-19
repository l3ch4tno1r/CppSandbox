#include <iostream>

#include "XMLParser.h"

int main()
{
	try
	{
		XMLParser parser("Ressources/scene.xml");

		SceneNode scene = parser.ParseFile();

		scene.ls_r();
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}