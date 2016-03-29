#include "AIController.h"
#include "World.h"


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
