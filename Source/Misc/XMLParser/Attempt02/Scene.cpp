#include "Scene.h"

BaseNode::BaseNode(const char * name, const Transform3D<float>& transform) :
	m_Name(name),
	m_Transform(transform)
{}
