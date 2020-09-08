#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "SceneNode.h"

class XMLNode
{
private:
	std::unordered_map<std::string, std::string> m_Attributes;
	std::vector<XMLNode> m_Children;

	friend class XMLParser;

public:
	XMLNode() = default;

	XMLNode(XMLNode&& other);

	const std::string& operator[](const std::string& key) { return m_Attributes[key]; }

	std::vector<XMLNode>& Children() { return m_Children; }

	const std::vector<XMLNode>& Children() const { return m_Children; }
};

class XMLParser
{
private:
	XMLNode m_Root;

public:
	SceneNode ParseFile(const std::string filepath) const;

	void ParseFile2(const std::string filepath);

	XMLNode& Root() { return m_Root; }	
};