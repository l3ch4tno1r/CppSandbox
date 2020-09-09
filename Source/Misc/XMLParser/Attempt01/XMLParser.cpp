#include "XMLParser.h"
#include "Source/ErrorHandling.h"

#include <fstream>
#include <sstream>
#include <stack>
#include <regex>

XMLNode::XMLNode(XMLNode && other) :
	m_Attributes(std::move(other.m_Attributes)),
	m_Children(std::move(other.m_Children))
{}

const std::string & XMLNode::operator[](const std::string& key) const
{
	auto it = m_Attributes.find(key);

	if (m_Attributes.end() == it)
		throw std::exception("XMLNode does not contain the specified key.");

	return it->second;
}

SceneNode XMLParser::ParseFile(const std::string filepath) const
{
	std::ifstream file(filepath);

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

void XMLParser::ParseFile2(const std::string filepath)
{
	std::ifstream file(filepath);

	if (!file)
		throw std::exception("File not found");

	std::stack<XMLNode*> stack;

	stack.push(&m_Root);

	bool first = true;
	std::string line;

	std::regex nodeStartRegEx("<Node>");
	std::regex nodeEndRegEx("</Node>");
	std::regex markupRegEx("<(\\w+)>([\\w\\d(-?\\d+\\.\\d* *)]+)</\\1>");

	while (std::getline(file, line))
	{
		XMLNode* current = stack.top();

		if (std::regex_search(line, nodeStartRegEx))
		{
			if (first)
			{
				first = false;
				continue;
			}

			current->m_Children.emplace_back();

			stack.push(&current->m_Children.back());

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

			current->m_Attributes[markupName] = markupData;
		}
	}

	ASSERT(stack.size() == 0);
}
