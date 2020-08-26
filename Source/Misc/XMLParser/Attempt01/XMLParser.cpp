#include "XMLParser.h"
#include "Utilities/ErrorHandling.h"

#include <fstream>
#include <sstream>
#include <stack>
#include <regex>

XMLParser::XMLParser(const char * filepath) :
	m_FilePath(filepath)
{}

SceneNode XMLParser::ParseFile() const
{
	std::ifstream file(m_FilePath);

	if (!file)
		throw std::exception("File not found");

	SceneNode root;
	std::stack<SceneNode*> stack;

	stack.push(&root);

	bool first = true;
	std::string line;

	std::regex nodeStartRegEx("<Node>");
	std::regex nodeEndRegEx("</Node>");
	std::regex markupRegEx("<(\\w+)>([\\w\\d(-?\\d+\\.\\d* *)]+)</\\1>");

	while (std::getline(file, line))
	{
		SceneNode* current = stack.top();

		if (std::regex_search(line, nodeStartRegEx))
		{
			if (first)
			{
				first = false;
				continue;
			}

			SceneNode& newNode = current->AddChild(SceneNode());

			stack.push(&newNode);

			continue;
		}

		if (std::regex_search(line, nodeEndRegEx))
		{
			stack.pop();

			if (0 == stack.size())
				break;
			else
				continue;
		}

		std::smatch matches;

		if (std::regex_search(line, matches, markupRegEx))
		{
			ASSERT(matches.size() == 3);

			if (matches.size() != 3)
				continue;

			std::string markupName = matches[1].str();
			std::string markupData = matches[2].str();

			if("Name" == markupName)
				current->Name() = markupData;

			if ("Geometry" == markupName)
				current->Geometry() = markupData;

			if ("Transformation" == markupName)
			{
				std::stringstream sstr(markupData);

				for (size_t i = 0; i < 16; ++i)
				{
					size_t I = i / 4;
					size_t J = i % 4;

					float temp;

					sstr >> temp;

					if (sstr.fail())
						throw std::exception("Transformation format incorrect.");

					current->Transform().mat(I, J) = temp;
				}
			}
		}
	}

	ASSERT(stack.size() == 0);

	return root;
}