#include "BaseNPC.h"

#include <iostream>

#include "Gizmos.h"
#include "World.h"

#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include <time.h>
#include <glm\gtc\random.hpp>

BaseNPC::BaseNPC(World* a_pWorld)
{
	m_pWorld = a_pWorld;
	m_vPosition = glm::vec3(0, 0, 0);

	m_fMoveSpeed = 5.0f;

	m_bAlive = true;

	m_fLastReportTime = 0.0f;
	m_fReportTime = 1.0f;
	m_fTimeAlive = 0.0f;

	m_uiFood = 30;
	m_uiWater = 30;
	m_uiRested = 30;

	m_uiNumberOfLogs = 0;
	m_uiRawLogs = 0;
	m_uiHarvestedFood = 0;
	m_fWanderTimer = 0.0f;
	m_wanderDest = glm::vec3(0, 0, 0);


	m_fLastFoodReductionTime = 0.0f;
	m_fLastWaterReductionTime = 0.0f;
	m_fLastRestedReductionTime = 0.0f;

	m_fFoodReductionTime = 1.0f;
	m_fWaterReductionTime = 1.5f;
	m_fRestedReductionTime = 2.0f;
}

BaseNPC::~BaseNPC()
{

}

void BaseNPC::update(float a_fdeltaTime)
{
	if (m_bAlive)
	{
		m_fTimeAlive += a_fdeltaTime;
		selectAction(a_fdeltaTime);
	
		calculateStatusChange();
		//reportStatus();

		checkAlive();

		m_fWanderTimer -= a_fdeltaTime;
	}
}

void BaseNPC::render()
{
	Gizmos::addAABBFilled(m_vPosition + glm::vec3(0, 2, 0), glm::vec3(0.5, 2, 0.5), glm::vec4(0.4, 0.4, 0.4, 1));
}

void BaseNPC::AssignNPCJob(NPCJob newJob)
{
	//Child, Farmer, Builder, Harvester, Dead, Unemployed
	if (newJob == Child)
	{
		m_NPCJob = Child;
	}
	else if (newJob == Farmer)
	{
		m_NPCJob = Farmer;
	}
	else if (newJob == Builder)
	{
		m_NPCJob = Builder;
	}
	else if (newJob == Harvester)
	{
		m_NPCJob = Harvester;
	}
	else if (newJob == Unemployed)
	{
		m_NPCJob = Unemployed;
	}
	else
	{
		std::cout << "ERROR: Assigned newJob = " << newJob << std::endl;
	}
}

void BaseNPC::collectWater(float a_fdeltaTime)
{
	if (travelTo(m_pWorld->getWaterLocation(), a_fdeltaTime))
	{
		if (m_pWorld->interactWithWater())
		{
			m_uiWater += 20;
			//std::cout << "Collected Water!" << std::endl;
		}
	}
}

void BaseNPC::collectFood(float a_fdeltaTime)
{
	//if (travelTo(m_pWorld->getFoodLocation(), a_fdeltaTime))
	//{
	//	if (m_pWorld->interactWithFood())
	//	{
	//		m_uiFood += 20;
	//		std::cout << "Collected Food!" << std::endl;
	//	}
	//}
	if (travelTo(m_pWorld->getStockpileLocation(), a_fdeltaTime))
	{
		if (m_pWorld->interactWithStockpile())
		{
			m_uiFood += m_pWorld->removeFoodFromStockpile(20);
			//std::cout << "Collected Food!" << std::endl;
		}
	}
}

void BaseNPC::rest(float a_fdeltaTime)
{
	if (travelTo(m_pWorld->getRestedLocation(), a_fdeltaTime))
	{
		if (m_pWorld->interactWithRested())
		{
			m_uiRested += 20;
			//std::cout << "Resting!" << std::endl;
		}
	}
}

void BaseNPC::harvestTree(float a_fdeltaTime)
{
	if (m_uiRawLogs <= 0)
	{
		chopTree(a_fdeltaTime);
	}
	else
	{
		depositStockpileLog(a_fdeltaTime);
	}
}

void BaseNPC::chopTree(float a_fdeltaTime)
{
	if (travelTo(m_pWorld->getTreeLocation(), a_fdeltaTime))
	{
		if (m_pWorld->interactWithTree())
		{
			m_uiRawLogs++;
			//std::cout << "Collected Log!" << std::endl;
		}
	}
}

void BaseNPC::buildHouse(float a_fdeltaTime)
{
	if (m_uiNumberOfLogs <= 0)
	{
		if (travelTo(m_pWorld->getStockpileLocation(), a_fdeltaTime))
		{
			if (m_pWorld->interactWithStockpile())
			{
				m_uiNumberOfLogs = m_pWorld->removeLogsFromStockpile(m_uiNumberOfLogs);
				//m_pWorld->removeLogsFromStockpile(m_uiNumberOfLogs);
				//m_uiNumberOfLogs++;
			}
		}
	}
	else// (travelTo(m_pWorld->getHouseLocation(), a_fdeltaTime))
	{
		if ((travelTo(m_pWorld->getHouseLocation(), a_fdeltaTime)))
		{
			if (m_uiNumberOfLogs <= 0)
			{
				//std::cout << "Don't have any logs to build with :(" << std::endl;
			}
			else
			{
				if (m_pWorld->interactWithHouse())
				{
					m_uiNumberOfLogs--;
					//std::cout << "Built House!" << std::endl;
					m_pWorld->addLogToHouse();
					//m_uiNumberOfLogs = m_pWorld->addLogToHouse(m_uiNumberOfLogs);
				}
			}
		}
	}
}

void BaseNPC::harvestFood(float a_fdeltaTime)
{
	if (m_uiHarvestedFood <= 0)
	{
		if (travelTo(m_pWorld->getFoodLocation(), a_fdeltaTime))
		{
			if (m_pWorld->interactWithFood())
			{
				m_uiHarvestedFood += 40;
				//std::cout << "Harvested Food!" << std::endl;
			}
		}
	}
	else
	{
		if (travelTo(m_pWorld->getStockpileLocation(), a_fdeltaTime))
		{
			depositStockpileFood(a_fdeltaTime);
		}
	}
}

void BaseNPC::depositStockpileLog(float a_fdeltaTime)
{
	if (travelTo(m_pWorld->getStockpileLocation(), a_fdeltaTime))
	{
		if (m_uiRawLogs <= 0)
		{
			//std::cout << "Don't have any logs to place in stockpile :(" << std::endl;
		}
		else
		{
			if (m_pWorld->interactWithStockpile())
			{
				//m_uiRawLogs--;
				m_uiRawLogs = m_pWorld->addLogsToStockpile(m_uiRawLogs);
				//std::cout << "Placed log in stockpile!" << std::endl;
				//m_pWorld->addLogsToStockpile(m_uiRawLogs);
			}
		}
	}
}

void BaseNPC::collectStockpileLog(float a_fdeltaTime)
{
	if (travelTo(m_pWorld->getStockpileLocation(), a_fdeltaTime))
	{
		if (m_pWorld->interactWithStockpile())
		{
			if (m_pWorld->getCurrentStockpileLogs() > 0)
			{
				m_pWorld->removeLogsFromStockpile(m_uiNumberOfLogs);
				m_uiNumberOfLogs++;
				std::cout << "Collected log from stockpile!" << std::endl;
			}
			else
			{
				std::cout << "Stockpile has no logs!" << std::endl;
			}
		}
	}
}

void BaseNPC::depositStockpileFood(float a_fdeltaTime)
{
	if (travelTo(m_pWorld->getStockpileLocation(), a_fdeltaTime))
	{
		if (m_uiHarvestedFood <= 0)
		{
			std::cout << "Don't have any Food to place in stockpile :(" << std::endl;
		}
		else
		{
			if (m_pWorld->interactWithStockpile())
			{
				m_uiHarvestedFood = m_pWorld->addFoodToStockpile(m_uiHarvestedFood);
				//std::cout << "Placed Food in stockpile!" << std::endl;
			}
		}
	}
}

void BaseNPC::wander(float a_fdeltaTime)
{

	if (m_fWanderTimer <= 0.0f)
	{
		float x = glm::linearRand(-10.0f, 10.0f);
		float z = glm::linearRand(-10.0f, 10.0f);
		m_wanderDest = glm::vec3(x, 0.0f, z);

		m_fWanderTimer = glm::linearRand(3.0f, 7.0f);
	}
	travelTo(m_wanderDest, a_fdeltaTime);
}

bool BaseNPC::travelTo(glm::vec3 a_vLoc, float a_fDeltaTime)
{
	glm::vec3 dirTo = (a_vLoc - m_vPosition);
	float distToPosition = glm::length(dirTo);
	dirTo = glm::normalize(dirTo);

	if (distToPosition <= 0.5)
		return true;
	else
	{
		float fMoveDist = a_fDeltaTime * m_fMoveSpeed;
		if (distToPosition < fMoveDist)
		{
			fMoveDist = distToPosition;
		}

		m_vPosition += dirTo * fMoveDist;
		return false;
	}
	
}

void BaseNPC::checkAlive()
{
	if (m_uiFood == 0)
	{
		std::cout << "Agent has starved to death!" << std::endl;
		m_bAlive = false;
		AssignNPCJob(Dead);
	}

	if (m_uiWater == 0)
	{
		std::cout << "Agent has dehydrated!" << std::endl;
		m_bAlive = false;
		AssignNPCJob(Dead);
	}

	if (m_uiRested == 0)
	{
		std::cout << "Agent has died due to exhaustion!!" << std::endl;
		m_bAlive = false;
		AssignNPCJob(Dead);
	}
}

void BaseNPC::reportStatus()
{
	float fCurrentTime = (float)glfwGetTime();
	if (fCurrentTime >= m_fLastReportTime + m_fReportTime)
	{
		m_fLastReportTime = fCurrentTime;
		std::cout << "{ Food: " << m_uiFood;
		std::cout << ", Water: " << m_uiWater;
		std::cout << ", Rest: " << m_uiRested;
		std::cout << ", Logs: " << m_uiNumberOfLogs;
		std::cout << ", RawLogs: " << m_uiRawLogs;
		std::cout << " }" << std::endl;
	}
}

void BaseNPC::calculateStatusChange()
{
	float fCurrentTime = (float)glfwGetTime();
	if (fCurrentTime >= m_fLastFoodReductionTime + m_fFoodReductionTime)
	{
		m_fLastFoodReductionTime = fCurrentTime;
		m_uiFood--;
	}
	if (fCurrentTime >= m_fLastWaterReductionTime + m_fWaterReductionTime)
	{
		m_fLastWaterReductionTime = fCurrentTime;
		m_uiWater--;
	}
	if (fCurrentTime >= m_fLastRestedReductionTime + m_fRestedReductionTime)
	{
		m_fLastRestedReductionTime = fCurrentTime;
		m_uiRested--;
	}
}

