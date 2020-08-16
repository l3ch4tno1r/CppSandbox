#include "XMLParser.h"
#include "Utilities/ErrorHandling.h"

#include <fstream>
#include <sstream>
#include <stack>
#include <regex>

XMLParser::XMLParser(const char * filepath) :
	m_FilePath(filepath)
{}

TreeNode XMLParser::ParseFile() const
{
	std::ifstream file(m_FilePath);

	if (!file)
		throw std::exception("File not found");

	TreeNode root;
	std::stack<TreeNode*> stack;

	stack.push(&root);

	bool first = true;
	std::string line;

	std::regex nodeStartRegEx("<Node>");
	std::regex nodeEndRegEx("<\Node>");
	std::regex nameRegEx("<Name>(\\w+)</Name>");
	std::regex transformRegEx("<Transformation>((-?\\d+\\.\\d* *){16})</Transformation>");
	std::regex geometryRegEx("<Geometry>(\\w+)</Geometry>");

	while (std::getline(file, line))
	{
		TreeNode* current = stack.top();

		std::string temp;
		std::stringstream sstr(line);

		sstr >> temp;

		if ("<Node>" == temp)
		{
			if (first)
			{
				first = false;
				continue;
			}

			TreeNode& newNode = current->AddChild(TreeNode());

			stack.push(&newNode);

			continue;
		}

		if ("</Node>" == temp)
		{
			stack.pop();

			if (0 == stack.size())
				break;
			else
				continue;
		}

		std::smatch matches;

		if (std::regex_search(temp, matches, nameRegEx))
		{
			if (matches.size() != 2)
				continue;

			current->Name() = matches[1].str();
		}

		if (std::regex_search(temp, matches, geometryRegEx))
		{
			if (matches.size() != 2)
				continue;

			current->Geometry() = matches[1].str();
		}
	}

	ASSERT(stack.size() == 0);

	return root;
}
