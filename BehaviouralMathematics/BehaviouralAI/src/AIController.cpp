#include "AIController.h"
#include "World.h"
#include "WorldObject.h"
#include "Buildings.h"

AIController::AIController(World* a_world)
{
	m_pWorld = a_world;

	m_farmerValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_farmerValue.setMinMaxValues(-1, 10);
	//m_farmerValue.setValue()	//Supply - Demand?
	UtilityScore* pFarmerValue = new UtilityScore();
	pFarmerValue->addUtilityValue(&m_farmerValue, 1.0f);
	m_pUtilityScoreMap["AssignFarmer"] = pFarmerValue;

	m_harvesterValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_harvesterValue.setMinMaxValues(0, 20);
	m_harvesterValue.setValue(m_pWorld->getCurrentStockpileLogs()); //Need for logs?
	UtilityScore* pHarvesterValue = new UtilityScore();
	pHarvesterValue->addUtilityValue(&m_harvesterValue, 0.70f);
	m_pUtilityScoreMap["AssignHarvester"] = pHarvesterValue;

	m_builderValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_builderValue.setMinMaxValues(0, 10);
	m_builderValue.setValue(CalculateHouseDemand());	// * (0,1) stockpile log value
	UtilityScore* pBuilderValue = new UtilityScore();
	pBuilderValue->addUtilityValue(&m_builderValue, 0.70f);
	m_pUtilityScoreMap["AssignBuilder"] = pBuilderValue;
	
}


AIController::~AIController()
{

}

void AIController::Update(float a_fdeltaTime)
{
	for (std::vector<UtilityNPC*>::const_iterator iter = m_pWorld->NPCVector.begin(); iter != m_pWorld->NPCVector.end(); iter++)
	{
		(*iter)->update(a_fdeltaTime);
	}
}

void AIController::Draw()
{
	for (std::vector<UtilityNPC*>::const_iterator iter = m_pWorld->NPCVector.begin(); iter != m_pWorld->NPCVector.end(); iter++)
	{
		(*iter)->render();
	}
}

void AIController::AddNPC()
{
	UtilityNPC* newNPC = new UtilityNPC(m_pWorld);
	m_pWorld->NPCVector.push_back(newNPC);
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

float AIController::CalculateFoodDemand()
{
	unsigned int AliveNPCs = 0;
	for (unsigned int i = 0; i < m_pWorld->NPCVector.size(); ++i)
	{
		if (m_pWorld->NPCVector[i]->getNPCJob() != Dead)
		{
			AliveNPCs++;
		}
	}
	//Food decreses once every 1 second
	return AliveNPCs / 1.0f;
}

float AIController::CalculateFoodSupply()
{
	//for each farmer, calculate time of trip, time to harvest, move speed,
	return 0.0f;
}

float AIController::CalculateWaterDemand()
{
	//Water decreses once every 1.5 second;
	return m_pWorld->NPCVector.size() / 1.5f;
}

float AIController::CalculateWaterSupply()
{
	return 0.0f;
}

float AIController::CalculateHouseDemand()
{
	unsigned int supply = 0;
	for (std::vector<House*>::const_iterator iter = m_pWorld->houseVector.begin(); iter != m_pWorld->houseVector.end(); iter++)
	{
		if ((*iter)->isHouseBuilt())
		{
			supply += 3;
		}
	}
	return (m_pWorld->NPCVector.size() - supply);
}

float AIController::CalculateHouseSupply()
{
	unsigned int supply = 0;
	for (std::vector<House*>::const_iterator iter = m_pWorld->houseVector.begin(); iter != m_pWorld->houseVector.end(); iter++)
	{
		if ((*iter)->isHouseBuilt())
		{
			supply += 3;
		}
	}
	return supply;
}

//Child, Farmer, Builder, Harvester, Dead, Unemployed
void AIController::CalculateNPCJobs()
{
	if (m_NPCJobVector.size() != m_pWorld->NPCVector.size())
	{
		std::cout << "NPCJobVector.size() != NPCList.size()" << std::endl;
	}

	//Calculated current jobs
	unsigned int CurChildren, CurFarmer, CurBuilder, CurHarvester, CurDead, CurUnpemployed;
	for (unsigned int i = 0; i < m_NPCJobVector.size(); ++i)
	{
		//Check if any children grew up 
		if (m_pWorld->NPCVector[i]->getNPCJob() == Child && m_pWorld->NPCVector[i]->getTimeAlive() > 20.0f)
		{
			m_pWorld->NPCVector[i]->AssignNPCJob(Unemployed);
		}
		//Calculated current jobs
		if (m_pWorld->NPCVector[i]->getNPCJob() == Child)
			CurChildren++;
		else if (m_pWorld->NPCVector[i]->getNPCJob() == Farmer)
			CurFarmer++;
		else if (m_pWorld->NPCVector[i]->getNPCJob() == Builder)
			CurBuilder++;
		else if (m_pWorld->NPCVector[i]->getNPCJob() == Harvester)
			CurHarvester++;
		else if (m_pWorld->NPCVector[i]->getNPCJob() == Dead)
			CurDead++;
		else if (m_pWorld->NPCVector[i]->getNPCJob() == Unemployed)
			CurUnpemployed++;
	}

	//Update MapValues
	//UtilityValue.SetValue(GetVarValue());
	m_farmerValue.setValue((m_pWorld->NPCVector.size() - CurDead) * 1.0f);	//Supply - Demand?
	m_harvesterValue.setValue(m_pWorld->getCurrentStockpileLogs()); //Need for logs?
	m_builderValue.setValue(CalculateHouseDemand());	// * (0,1) stockpile log value

	//Check Available workers
	unsigned int aliveWorkers = m_pWorld->NPCVector.size() - CurChildren - CurDead;
	//Calculate ideal worker jobs
	unsigned int idealFarmer = 0, idealBuilder = 0, idealHarvester = 0;//, idealChildren = 0, idealDead = 0, idealUnpemployed = 0;
	for (unsigned int i = 0; i < aliveWorkers; ++i)
	{
		float fBestScore = 0.0f;
		std::string strBestAction;
		for (auto score : m_pUtilityScoreMap)
		{
			float fThisScore = score.second->getUtilityScore();
			if (fThisScore > fBestScore)
			{
				fBestScore = fThisScore;
				strBestAction = score.first;
			}
		}

		if (strBestAction == "AssignFarmer")
		{
			idealFarmer++;
			m_farmerValue.setValue(-CalculateFoodDemand() + idealFarmer * 3);
		}
		else if (strBestAction == "AssignHarvester")
		{
			idealHarvester++;
			m_harvesterValue.setValue(m_pWorld->getCurrentStockpileLogs() + (idealHarvester * 5));
		}
		else if (strBestAction == "AssignBuilder");
		{
			idealBuilder++;
			m_builderValue.setValue(CalculateHouseDemand() - (idealBuilder * 3));
		}
	}

	//if current numbers != ideal, Reassign jobs
	if ((CurBuilder != idealBuilder) || (CurFarmer != idealFarmer) || (CurHarvester != idealHarvester))
	{
		//Find workers who need reassign
		std::vector<UtilityNPC*> ReassignNPC;
		for (std::vector<UtilityNPC*>::const_iterator iter = m_pWorld->NPCVector.begin(); iter != m_pWorld->NPCVector.end(); iter++)
		{
			if ((*iter)->getNPCJob() == Builder && (CurBuilder > idealBuilder))
			{
				ReassignNPC.push_back((*iter));
			}
			else if ((*iter)->getNPCJob() == Farmer && (CurFarmer > idealFarmer))
			{
				ReassignNPC.push_back((*iter));
			}
			else if ((*iter)->getNPCJob() == Harvester && (CurHarvester > idealHarvester))
			{
				ReassignNPC.push_back((*iter));
			}
			else if ((*iter)->getNPCJob() == Unemployed)
			{
				ReassignNPC.push_back((*iter));
			}
		}
		//reassign
		while (!ReassignNPC.empty())
		{
			if (idealBuilder > CurBuilder)
			{
				ReassignNPC.back()->AssignNPCJob(Builder);
				ReassignNPC.pop_back();
			}
			else if (idealFarmer > CurFarmer)
			{
				ReassignNPC.back()->AssignNPCJob(Farmer);
				ReassignNPC.pop_back();
			}
			else if (idealHarvester > CurHarvester)
			{
				ReassignNPC.back()->AssignNPCJob(Harvester);
				ReassignNPC.pop_back();
			}
		}

	}
}
