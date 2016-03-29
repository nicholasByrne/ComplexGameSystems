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

private:
	World* m_pWorld;
};

