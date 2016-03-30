#include "AIController.h"
#include "World.h"
#include "WorldObject.h"
#include "Buildings.h"

AIController::AIController(World* a_world)
{
	m_pWorld = a_world;
}


AIController::~AIController()
{

}

void AIController::Update(float a_fdeltaTime)
{
	for (std::list<UtilityNPC*>::const_iterator iter = m_pWorld->NPCList.begin(); iter != m_pWorld->NPCList.end(); iter++)
	{
		(*iter)->update(a_fdeltaTime);
	}
}

void AIController::Draw()
{
	for (std::list<UtilityNPC*>::const_iterator iter = m_pWorld->NPCList.begin(); iter != m_pWorld->NPCList.end(); iter++)
	{
		(*iter)->render();
	}
}

void AIController::AddNPC()
{
	UtilityNPC* newNPC = new UtilityNPC(m_pWorld);
	m_pWorld->NPCList.push_back(newNPC);
}

glm::vec3 AIController::FindClosestFood(glm::vec3 a_myLoc)
{
	glm::vec3 closest = m_pWorld->foodVector[0]->getLocation();
	for (std::vector<Food*>::iterator it = m_pWorld->foodVector.begin(); it != m_pWorld->foodVector.end(); it++)
	{
		if (glm::distance(a_myLoc, (*it)->getLocation()) < glm::distance(a_myLoc, closest));
		{
			closest = (*it)->getLocation();
		}
	}
	return closest;
}

glm::vec3 AIController::FindClosestWater(glm::vec3 a_myLoc)
{
	glm::vec3 closest = m_pWorld->waterVector[0]->getLocation();
	for (std::vector<Water*>::iterator it = m_pWorld->waterVector.begin(); it != m_pWorld->waterVector.end(); it++)
	{
		if (glm::distance(a_myLoc, (*it)->getLocation()) < glm::distance(a_myLoc, closest));
		{
			closest = (*it)->getLocation();
		}
	}
	return closest;
}

glm::vec3 AIController::FindClosestRest(glm::vec3 a_myLoc)
{
	glm::vec3 closest = m_pWorld->restVector[0]->getLocation();
	for (std::vector<Rest*>::iterator it = m_pWorld->restVector.begin(); it != m_pWorld->restVector.end(); it++)
	{
		if (glm::distance(a_myLoc, (*it)->getLocation()) < glm::distance(a_myLoc, closest));
		{
			closest = (*it)->getLocation();
		}
	}
	return closest;
}

glm::vec3 AIController::FindClosestTree(glm::vec3 a_myLoc)
{
	glm::vec3 closest = m_pWorld->treeVector[0]->getLocation();
	for (std::vector<Tree*>::iterator it = m_pWorld->treeVector.begin(); it != m_pWorld->treeVector.end(); it++)
	{
		if (glm::distance(a_myLoc, (*it)->getLocation()) < glm::distance(a_myLoc, closest));
		{
			closest = (*it)->getLocation();
		}
	}
	return closest;
}
