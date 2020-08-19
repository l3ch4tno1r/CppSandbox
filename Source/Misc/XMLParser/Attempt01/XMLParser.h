#pragma once

#include <string>

#include "SceneNode.h"

class XMLParser
{
private:
	std::string m_FilePath;

public:
	XMLParser(const char* filepath);

	SceneNode ParseFile() const;
};