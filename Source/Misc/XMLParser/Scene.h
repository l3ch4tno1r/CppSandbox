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
};


