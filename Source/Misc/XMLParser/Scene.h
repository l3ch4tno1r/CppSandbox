#pragma once

#include <string>

#include "Geometry/Geometry3D/Transform3D.h"

class BaseNode
{
private:
	std::string m_Name;
	Transform3D<float> m_Transform;

public:
	BaseNode(const char* name, const Transform3D<float>& transform);

	virtual ~BaseNode() {}

	const std::string& Name() const             { return m_Name; }
	const Transform3D<float>& Transform() const { return m_Transform; }
};

class GeometryNode : public BaseNode
{
private:
	std::string m_Geometry;

public:
	GeometryNode(const char* name, const char* geometry, const Transform3D<float>& transform) :
		BaseNode(name, transform),
		m_Geometry(geometry)
	{}

	const std::string& Geometry() const { return m_Geometry; }
};

class GroupNode : public BaseNode
{};