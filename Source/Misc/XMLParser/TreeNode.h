#pragma once

#include <string>
#include <vector>

class TreeNode
{
private:
	std::string m_Name;
	std::string m_Geometry;
	std::string m_Transform;

	std::vector<TreeNode> m_Children;

public:
	TreeNode(const char* name, const char* geometry = nullptr);
};
