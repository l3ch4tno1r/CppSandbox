#pragma once

#include <string>

#include "TreeNode.h"

class XMLParser
{
private:
	std::string m_FilePath;

public:
	XMLParser(const char* filepath);

	TreeNode ParseFile() const;
};