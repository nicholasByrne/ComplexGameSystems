#pragma once
#include <glm/glm.hpp>

class WorldObject
{
public:
	WorldObject();
	~WorldObject();

	virtual bool InteractWith();
	glm::vec3 getLocation() const { return m_location; }

	virtual void Update(float a_fdeltaTime);
	virtual void Render();

protected:

	glm::vec3 m_location;

	float m_fInteractTime;
	float m_fLastInteractTime;
};

