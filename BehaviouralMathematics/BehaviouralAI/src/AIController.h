#pragma once
//Behavioural Mathematics Tutorial
#include "UtilityNPC.h"
#include <glm/glm.hpp>
#include <list>
#include <map>

class World;

class AIController
{
public:
	AIController(World* a_world);
	~AIController();

	//std::list<UtilityNPC*> NPCList;

	void Update(float a_fdeltaTime);
	void Draw();

	void AddNPC();

	glm::vec3 FindClosestFood(glm::vec3 a_myLoc);
	glm::vec3 FindClosestWater(glm::vec3 a_myLoc);
	glm::vec3 FindClosestRest(glm::vec3 a_myLoc);
	glm::vec3 FindClosestTree(glm::vec3 a_myLoc);

	//make a find building to build function?
	//makes rest/water/food dist functions

private:
	World* m_pWorld;
};

