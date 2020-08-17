#include "SceneNode.h"

#include <iostream>

const int indentScale = 2;

SceneNode::SceneNode(const SceneNode& other) :
	m_Name(other.m_Name),
	m_Transform(other.m_Transform),
	m_Geometry(other.m_Geometry),
	m_Children(other.m_Children)
{}

SceneNode::SceneNode(SceneNode&& other) :
	m_Name(std::move(other.m_Name)),
	m_Transform(std::move(other.m_Transform)),
	m_Geometry(std::move(other.m_Geometry)),
	m_Children(std::move(other.m_Children))
{}

SceneNode& SceneNode::AddChild(const SceneNode& child)
{
	m_Children.push_back(child);

	return m_Children[m_Children.size() - 1];
}

SceneNode& SceneNode::AddChild(SceneNode&& child)
{
	m_Children.push_back(std::move(child));

	return m_Children[m_Children.size() - 1];
}

void SceneNode::ls_r(size_t indent) const
{
	std::string _indent(indentScale * indent++, ' ');

	std::cout << _indent << m_Name;

	if (!m_Geometry.empty())
		std::cout << " (" << m_Geometry << ')';

	//std::cout << " - " << m_Transform << std::endl;
	std::cout << std::endl;

	for (const SceneNode& e : m_Children)
		e.ls_r(indent);
}
