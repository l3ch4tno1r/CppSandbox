#include <iostream>
#include <stack>

#include "XMLParser.h"

int main()
{
	try
	{
		XMLParser parser;

		SceneNode scene = parser.ParseFile("Ressources/scene.xml");

		scene.ls_r();

		std::cout << std::endl;

		parser.ParseFile2("Ressources/scene.xml");

		XMLNode root = std::move(parser.Root());

		std::stack<XMLNode*> stack;

		stack.push(&root);

		while (!stack.empty())
		{
			XMLNode* current = stack.top();

			for (auto& child : current->Children())
				stack.push(&child);

			stack.pop();
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}