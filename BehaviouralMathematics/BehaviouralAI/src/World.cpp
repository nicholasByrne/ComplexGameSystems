#include "World.h"
#include "Gizmos.h"

#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>

#include <iostream>

#include "Buildings.h"

World::World()
{
	m_houseLocation = glm::vec3(10, 0, -10);
	m_treeLocation = glm::vec3(-10, 0, -10);

	m_foodLocation = glm::vec3(10, 0, 10);
	m_waterLocation = glm::vec3(0, 0, 10);
	m_restedLocation = glm::vec3(-10, 0, 10);
	m_stockpileLocation = glm::vec3(5, 0, 5);

	BuildNewFood(m_foodLocation);
	BuildNewHouse(m_houseLocation);
	BuildNewRest(m_restedLocation);
	BuildNewTree(m_treeLocation);
	BuildNewWater(m_waterLocation);
	

	m_uiHouseCurrentLogs = 4;
	m_uiHouseLogsRequired = 10;
	m_uiStockpileCurrentLogs = 0;
	m_uiStockpileMaxLogs = 10;
	m_uiStockpileCurrentFood = 20;
	m_uiStockpileMaxFood = 200;

	m_fRestedInteractTime = 1.0f;
	m_fWaterInteractTime = 2.0f;
	m_fFoodInteractTime =  2.0f;

	m_fHouseInteractTime = 3.0;
	m_fTreeInteractTime = 1.0f;

	m_fLastFoodInteractTime = 0.0f;
	m_fLastWaterInteractTime = 0.0f;
	m_fLastRestedInteractTime = 0.0f;
	m_fLastHouseInteractTime = 0.0f;
	m_fLastTreeInteractTime = 0.0f;

}

World::~World()
{

}

void World::update(float a_fdeltaTime)
{
	//Update vectors
	for (std::vector<House*>::const_iterator iter = houseVector.begin(); iter != houseVector.end(); iter++)
	{
		(*iter)->Update(a_fdeltaTime);
	}
	for (std::vector<Food*>::const_iterator iter = foodVector.begin(); iter != foodVector.end(); iter++)
	{
		(*iter)->Update(a_fdeltaTime);
	}
	for (std::vector<Water*>::const_iterator iter = waterVector.begin(); iter != waterVector.end(); iter++)
	{
		(*iter)->Update(a_fdeltaTime);
	}
	for (std::vector<Rest*>::const_iterator iter = restVector.begin(); iter != restVector.end(); iter++)
	{
		(*iter)->Update(a_fdeltaTime);
	}
	for (std::vector<Tree*>::const_iterator iter = treeVector.begin(); iter != treeVector.end(); iter++)
	{
		(*iter)->Update(a_fdeltaTime);
	}
}

void World::render()
{
	Gizmos::addSphere(m_foodLocation, 1, 8, 8, glm::vec4(1, 0, 0, 1));
	Gizmos::addSphere(m_waterLocation, 1, 8, 8, glm::vec4(0, 0, 1, 1));
	Gizmos::addSphere(m_restedLocation, 1, 8, 8, glm::vec4(0, 1, 1, 1));


	float m_fHouseHeight = 3 * ((float)m_uiHouseCurrentLogs / m_uiHouseLogsRequired);
	glm::vec4 houseColor = (m_uiHouseCurrentLogs >= m_uiHouseLogsRequired) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 1, 0, 1);

	Gizmos::addAABBFilled(m_houseLocation + glm::vec3(0, m_fHouseHeight, 0), glm::vec3(3, m_fHouseHeight, 2), houseColor);

	Gizmos::addCylinderFilled(m_treeLocation, 1, 2, 8, glm::vec4(0, 1, 0, 1));

	float m_fStockpileHeight = 1 * ((float)m_uiStockpileCurrentLogs) / 4;
	Gizmos::addAABBFilled(m_stockpileLocation + glm::vec3(0, m_fStockpileHeight, 0), glm::vec3(3, m_fStockpileHeight, 2), glm::vec4(1, 1, 1, 1));

	//Render vectors
	for (std::vector<House*>::const_iterator iter = houseVector.begin(); iter != houseVector.end(); iter++)
	{
		(*iter)->Render();
	}
	for (std::vector<Food*>::const_iterator iter = foodVector.begin(); iter != foodVector.end(); iter++)
	{
		(*iter)->Render();
	}
	for (std::vector<Water*>::const_iterator iter = waterVector.begin(); iter != waterVector.end(); iter++)
	{
		(*iter)->Render();
	}
	for (std::vector<Rest*>::const_iterator iter = restVector.begin(); iter != restVector.end(); iter++)
	{
		(*iter)->Render();
	}
	for (std::vector<Tree*>::const_iterator iter = treeVector.begin(); iter != treeVector.end(); iter++)
	{
		(*iter)->Render();
	}
}

void World::addLogToHouse()
{
	if (m_uiHouseCurrentLogs < m_uiHouseLogsRequired)
		m_uiHouseCurrentLogs++;
}

float World::addLogsToStockpile(unsigned int logs)
{
	if (m_uiStockpileCurrentLogs < m_uiStockpileMaxLogs)
	{
		if (m_uiStockpileCurrentLogs + logs <= m_uiStockpileMaxLogs)
		{
			m_uiStockpileCurrentLogs += logs;
			return 0;
		}
		else
		{
			while (m_uiStockpileCurrentLogs < m_uiStockpileMaxLogs)
			{
				m_uiStockpileCurrentLogs++;
				logs--;
			}
			return logs;
		}
	}
	else
		return logs;

	//m_uiStockpileCurrentLogs += logs;
}

float World::removeLogsFromStockpile(unsigned int logs)
{
	if (m_uiStockpileCurrentLogs > 0)
	{
		unsigned int Takeout = 0;
		while (m_uiStockpileCurrentLogs > 0)
		{
			m_uiStockpileCurrentLogs--;
			Takeout++;
		}
		return Takeout;
	}
	else return 0;
}

float World::addFoodToStockpile(unsigned int amount)
{
	if (m_uiStockpileCurrentFood < m_uiStockpileMaxFood)
	{
		if (m_uiStockpileCurrentFood + amount <= m_uiStockpileMaxFood)
		{
			m_uiStockpileCurrentFood += amount;
			return 0;
		}
		else
		{
			while (m_uiStockpileCurrentFood < m_uiStockpileMaxFood || amount > 0)
			{
				m_uiStockpileCurrentFood++;
				amount--;
			}
			return amount;
		}
	}
	else
		return amount;
}

float World::removeFoodFromStockpile(unsigned int amount)
{
	if (m_uiStockpileCurrentFood > 0)
	{
		if (m_uiStockpileCurrentFood >= amount)
		{
			m_uiStockpileCurrentFood -= amount;
			return amount;
		}
		else
		{
			unsigned int Takeout = 0;
			while (m_uiStockpileCurrentFood > 0)
			{
				m_uiStockpileCurrentFood--;
				Takeout++;
			}
			return Takeout;
		}
	}
	else return 0;
}

bool World::interactWithFood()
{
	float fCurrentTime = (float)glfwGetTime();
	if (fCurrentTime >= m_fLastFoodInteractTime + m_fFoodInteractTime)
	{
		m_fLastFoodInteractTime = fCurrentTime;
		return true;
	}
	return false;
}

bool World::interactWithWater()
{
	float fCurrentTime = (float)glfwGetTime();

	if (fCurrentTime >= m_fLastWaterInteractTime + m_fWaterInteractTime)
	{
		m_fLastWaterInteractTime = fCurrentTime;
		return true;
	}

	return false;
}

bool World::interactWithRested()
{
	float fCurrentTime = (float)glfwGetTime();
	if (fCurrentTime >= m_fLastRestedInteractTime + m_fRestedInteractTime)
	{
		m_fLastRestedInteractTime = fCurrentTime;
		return true;
	}
	return false;
}

bool World::interactWithTree()
{
	float fCurrentTime = (float)glfwGetTime();
	if (fCurrentTime >= m_fLastTreeInteractTime + m_fTreeInteractTime)
	{
		m_fLastTreeInteractTime = fCurrentTime;
		return true;
	}
	return false;
}

bool World::interactWithHouse()
{
	float fCurrentTime = (float)glfwGetTime();
	if (fCurrentTime >= m_fLastHouseInteractTime + m_fHouseInteractTime)
	{
		m_fLastHouseInteractTime = fCurrentTime;
		return true;
	}
	return false;
}

bool World::interactWithStockpile()
{
	float fCurrentTime = (float)glfwGetTime();
	if (fCurrentTime >= m_fLastStockpileInteractTime + m_fStockpileInteractTime)
	{
		m_fLastStockpileInteractTime = fCurrentTime;
		return true;
	}
	return false;
}

void World::BuildNewFood(glm::vec3 a_location)
{
	Food* newFood = new Food(a_location);
	foodVector.push_back(newFood);
}

void World::BuildNewWater(glm::vec3 a_location)
{
	Water* newWater = new Water(a_location);
	waterVector.push_back(newWater);
}

void World::BuildNewRest(glm::vec3 a_location)
{
	Rest* newRest = new Rest(a_location);
	restVector.push_back(newRest);
}

void World::BuildNewTree(glm::vec3 a_location)
{
	Tree* newTree = new Tree(a_location);
	treeVector.push_back(newTree);
}

void World::BuildNewHouse(glm::vec3 a_location)
{
	House* newHouse = new House(a_location);
	houseVector.push_back(newHouse);
}

glm::vec3 World::getRestedLocation() const
{
	return m_restedLocation;
}

glm::vec3 World::getStockpileLocation() const
{
	return m_stockpileLocation;
}

glm::vec3 World::getFoodLocation() const
{
	return m_foodLocation;
}

glm::vec3 World::getWaterLocation() const
{
	return m_waterLocation;
}

glm::vec3 World::getTreeLocation() const
{
	return m_treeLocation;
}


glm::vec3 World::getHouseLocation() const
{
	return m_houseLocation;
}


glm::vec3 World::FindClosestFood(glm::vec3 a_myLoc)
{
	glm::vec3 closest = foodVector[0]->getLocation();
	for (std::vector<Food*>::iterator it = foodVector.begin(); it != foodVector.end(); it++)
	{
		if (glm::distance(a_myLoc, (*it)->getLocation()) < glm::distance(a_myLoc, closest));
		{
			closest = (*it)->getLocation();
		}
	}
	return closest;
}

glm::vec3 World::FindClosestWater(glm::vec3 a_myLoc)
{
	glm::vec3 closest = waterVector[0]->getLocation();
	for (std::vector<Water*>::iterator it = waterVector.begin(); it != waterVector.end(); it++)
	{
		if (glm::distance(a_myLoc, (*it)->getLocation()) < glm::distance(a_myLoc, closest));
		{
			closest = (*it)->getLocation();
		}
	}
	return closest;
}

glm::vec3 World::FindClosestRest(glm::vec3 a_myLoc)
{
	glm::vec3 closest = restVector[0]->getLocation();
	for (std::vector<Rest*>::iterator it = restVector.begin(); it != restVector.end(); it++)
	{
		if (glm::distance(a_myLoc, (*it)->getLocation()) < glm::distance(a_myLoc, closest));
		{
			closest = (*it)->getLocation();
		}
	}
	return closest;
}

glm::vec3 World::FindClosestTree(glm::vec3 a_myLoc)
{
	glm::vec3 closest = treeVector[0]->getLocation();
	for (std::vector<Tree*>::iterator it = treeVector.begin(); it != treeVector.end(); it++)
	{
		if (glm::distance(a_myLoc, (*it)->getLocation()) < glm::distance(a_myLoc, closest));
		{
			closest = (*it)->getLocation();
		}
	}
	return closest;
}
