#pragma once

#include <string>
#include <vector>

class TreeNode
{
private:
	std::string m_Name;
	std::string m_Transform;
	std::string m_Geometry;

	std::vector<TreeNode> m_Children;

public:
	TreeNode() = default;

	TreeNode(const TreeNode& other);

	TreeNode(TreeNode&& other);

	TreeNode& AddChild(const TreeNode& child);

	TreeNode& AddChild(TreeNode&& child);

	template<typename ...Args>
	TreeNode& AddChildEmplace(Args&& ...args)
	{
		m_Children.emplace_back(std::forward<Args>(args)...);

		return m_Children[m_Children.size() - 1];
	}

	void ls_r(size_t indent = 0) const;

	std::string& Name()      { return m_Name; }
	std::string& Transform() { return m_Transform; }
	std::string& Geometry()  { return m_Geometry; }

	const std::string& Name() const      { return m_Name; }
	const std::string& Transform() const { return m_Transform; }
	const std::string& Geometry() const  { return m_Geometry; }

	const std::vector<TreeNode>& Children() const { return m_Children; }
};
