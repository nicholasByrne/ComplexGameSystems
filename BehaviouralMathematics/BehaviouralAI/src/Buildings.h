#pragma once
#include "WorldObject.h"
#include <glm\glm.hpp>


class House : public WorldObject
{
public:
	House(glm::vec3 a_location);
	~House();

	void Update(float a_fdeltaTime);
	void Render();

	unsigned int getCurrentLogs() { return m_uiCurrentLogs; };
	unsigned int getRequiredLogs() { return m_uiRequiredLogs; }
	void addLogToHouse();

	bool isHouseBuilt() { return m_uiCurrentLogs == m_uiRequiredLogs; }

protected:
	unsigned int m_uiCurrentLogs;
	unsigned int m_uiRequiredLogs;

};


class Food : public WorldObject
{
public:
	Food(glm::vec3 a_location);
	~Food();

	void Update(float a_fdeltaTime);
	void Render();
};

class Water : public WorldObject
{
public:
	Water(glm::vec3 a_location);
	~Water();

	void Update(float a_fdeltaTime);
	void Render();
};

class Rest : public WorldObject
{
public:
	Rest(glm::vec3 a_location);
	~Rest();

	void Update(float a_fdeltaTime);
	void Render();
};

class Tree : public WorldObject
{
public:
	Tree(glm::vec3 a_location);
	~Tree();

	void Update(float a_fdeltaTime);
	void Render();
};