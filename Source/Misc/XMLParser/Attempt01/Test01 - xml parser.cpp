#include <iostream>
#include <stack>
#include <sstream>

#include "XMLParser.h"

Transform3D<float> StrToTransform(const std::string& data)
{
	std::stringstream sstr(data);

	Transform3D<float> result;

	for (size_t i = 0; i < 16; ++i)
	{
		size_t I = i / 4;
		size_t J = i % 4;

		float temp;

		sstr >> temp;

		if (sstr.fail())
			throw std::exception("Transformation format incorrect.");

		result.mat(I, J) = temp;
	}

	return result;
}

SceneNode XMLNodeToSceneNode(const XMLNode& node)
{
	SceneNode scene;

	scene.Name() = node["Name"];
	scene.Transform() = StrToTransform(node["Transformation"]);

	if(node.ContainsAttribute("Geometry"))
	{ }
	else
	{

	}

	return scene;
}

int main()
{
	try
	{
		XMLParser parser;

		SceneNode scene1 = parser.ParseFile("Ressources/scene.xml");

		scene1.ls_r();

		std::cout << "------------------------" << std::endl;

		parser.ParseFile2("Ressources/scene.xml");

		XMLNode root = std::move(parser.Root());
		SceneNode scene2 = XMLNodeToSceneNode(root);

		/*
		if(root.ContainsAttribute("Name"))
			scene2.Name() = root["Name"];

		std::stack<XMLNode*> stack;

		stack.push(&root);

		while (!stack.empty())
		{
			XMLNode* current = stack.top();

			stack.pop();

			for (auto att : current->Attributes())
				std::cout << att.first << " = " << att.second << std::endl;

			for (auto it = current->Children().rbegin(); it != current->Children().rend(); ++it)
				stack.push(&(*it));
		}
		*/
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}