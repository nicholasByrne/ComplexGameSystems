#include "Buildings.h"
#include "Gizmos.h"

House::House(glm::vec3 a_location)
{
	m_location = a_location;
	m_fInteractTime = 3.0f;
	m_fLastInteractTime = 0.0f;

	m_uiCurrentLogs = 4;
	m_uiRequiredLogs = 10;
}

House::~House()
{

}

void House::Update(float a_fdeltaTime)
{
}

void House::Render()
{
	float m_fHouseHeight = 3 * ((float)m_uiCurrentLogs / m_uiRequiredLogs);
	glm::vec4 houseColor = (m_uiCurrentLogs >= m_uiRequiredLogs) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 1, 0, 1);

	Gizmos::addAABBFilled(m_location + glm::vec3(0, m_fHouseHeight, 0), glm::vec3(3, m_fHouseHeight, 2), houseColor);
}

void House::addLogToHouse()
{
	if (m_uiCurrentLogs < m_uiRequiredLogs)
		m_uiCurrentLogs++;
}

Food::Food(glm::vec3 a_location)
{
	m_location = a_location;
	m_fInteractTime = 2.0f;
	m_fLastInteractTime = 0.0f;
}

Food::~Food()
{
}

void Food::Update(float a_fdeltaTime)
{
}

void Food::Render()
{
	Gizmos::addSphere(m_location, 1, 8, 8, glm::vec4(1, 0, 0, 1));
}

Water::Water(glm::vec3 a_location)
{
	m_location = a_location;
	m_fInteractTime = 2.0f;
	m_fLastInteractTime = 0.0f;
}

Water::~Water()
{
}

void Water::Update(float a_fdeltaTime)
{
}

void Water::Render()
{
	Gizmos::addSphere(m_location, 1, 8, 8, glm::vec4(0, 0, 1, 1));
}

Rest::Rest(glm::vec3 a_location)
{
	m_location = a_location;
	m_fInteractTime = 1.0f;
	m_fLastInteractTime = 0.0f;
}

Rest::~Rest()
{
}

void Rest::Update(float a_fdeltaTime)
{
}

void Rest::Render()
{
	Gizmos::addSphere(m_location, 1, 8, 8, glm::vec4(0, 1, 1, 1));
}


Tree::Tree(glm::vec3 a_location)
{
	m_location = a_location;
	m_fInteractTime = 1.0f;
	m_fLastInteractTime = 0.0f;
}

Tree::~Tree()
{
}

void Tree::Update(float a_fdeltaTime)
{
}

void Tree::Render()
{
	Gizmos::addCylinderFilled(m_location, 1, 2, 8, glm::vec4(0, 1, 0, 1));
}
