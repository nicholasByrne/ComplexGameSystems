#pragma once
//Behavioural Mathematics Tutorial
#include "UtilityNPC.h"
#include <glm/glm.hpp>
#include <list>
#include <map>
#include <vector>
#include <iostream>

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

	float CalculateFoodDemand();
	float CalculateFoodSupply();

	float CalculateWaterDemand();
	float CalculateWaterSupply();

	float CalculateHouseDemand();
	float CalculateHouseSupply();

	void CalculateNPCJobs();
	

private:
	World* m_pWorld;

	std::map<std::string, UtilityScore*> m_pUtilityScoreMap;

	UtilityValue m_farmerValue;
	UtilityValue m_builderValue;
	UtilityValue m_harvesterValue;

	//enum NPCJob { Child, Farmer, Builder, Harvester, Dead Unemployed };
	std::vector<NPCJob> m_NPCJobVector;
	
	bool canSpawn = true;
	float reassignTimer = 0.0f;

};







//Change Harvester Desire to be = to
// max - current(Linear?)



//make remove logstockpile return bool, confirm actually has log


//Fix NPC map //Harvesting food


//make a find building to build function?
//makes rest/water/food dist functions
//Babies // Take time to grow? while not working
//Give npcs coloured cube on head for job?
//BaseNPC // make wander function (take in size of grid?)
//Set map according to enum (job?)


//Make AIController use utility scores?
//Assign builders/Farmers/Harvesters according to demand
//Demands = food/water?/Houses/Logs
//Vector[npcs] of enums for jobs?
//Child/Farmer/Harvester/Builder/Dead
//
//
//