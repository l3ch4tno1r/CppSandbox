#pragma once

#include <string>
#include <vector>

#include "Geometry/Geometry3D/Transform3D.h"

class SceneNode
{
private:
	std::string m_Name;
	std::string m_Geometry;

	std::vector<SceneNode> m_Children;

	Transform3D<float> m_Transform;

public:
	SceneNode() = default;

	SceneNode(const SceneNode& other);

	SceneNode(SceneNode&& other);

	SceneNode& AddChild(const SceneNode& child);

	SceneNode& AddChild(SceneNode&& child);

	template<typename ...Args>
	SceneNode& AddChildEmplace(Args&& ...args)
	{
		m_Children.emplace_back(std::forward<Args>(args)...);

		return m_Children[m_Children.size() - 1];
	}

	void ls_r(size_t indent = 0) const;

	std::string& Name()      { return m_Name; }
	std::string& Geometry()  { return m_Geometry; }

	const std::string& Name() const      { return m_Name; }
	const std::string& Geometry() const  { return m_Geometry; }

	Transform3D<float>& Transform() { return m_Transform; }
	const Transform3D<float>& Transform() const { return m_Transform; }

	const std::vector<SceneNode>& Children() const { return m_Children; }
};
