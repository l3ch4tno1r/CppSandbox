#include "TreeNode.h"

#include <iostream>

const int indentScale = 2;

TreeNode::TreeNode(const TreeNode& other) :
	m_Name(other.m_Name),
	m_Transform(other.m_Transform),
	m_Geometry(other.m_Geometry),
	m_Children(other.m_Children)
{}

TreeNode::TreeNode(TreeNode&& other) :
	m_Name(std::move(other.m_Name)),
	m_Transform(std::move(other.m_Transform)),
	m_Geometry(std::move(other.m_Geometry)),
	m_Children(std::move(other.m_Children))
{}

TreeNode& TreeNode::AddChild(const TreeNode& child)
{
	m_Children.push_back(child);

	return m_Children[m_Children.size() - 1];
}

TreeNode& TreeNode::AddChild(TreeNode&& child)
{
	m_Children.push_back(std::move(child));

	return m_Children[m_Children.size() - 1];
}

void TreeNode::ls_r(size_t indent) const
{
	std::string _indent(indentScale * indent++, ' ');

	std::cout << _indent << m_Name;

	if (!m_Geometry.empty())
		std::cout << " - " << m_Geometry;

	std::cout << std::endl;

	for (const TreeNode& e : m_Children)
		e.ls_r(indent);
}
