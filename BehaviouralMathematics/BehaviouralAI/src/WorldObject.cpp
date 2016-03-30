#include "WorldObject.h"
#include "Gizmos.h"

#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>


WorldObject::WorldObject()
{
}


WorldObject::~WorldObject()
{
}

bool WorldObject::InteractWith()
{
	float fCurrentTime = (float)glfwGetTime();
	if (fCurrentTime >= m_fLastInteractTime + m_fInteractTime)
	{
		m_fLastInteractTime = fCurrentTime;
		return true;
	}
	return false;
}

void WorldObject::Update(float a_fdeltaTime)
{
}

void WorldObject::Render()
{
}
