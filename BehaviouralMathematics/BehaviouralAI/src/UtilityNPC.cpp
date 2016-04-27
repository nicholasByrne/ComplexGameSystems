#include "UtilityNPC.h"
#include "World.h"
#include <iostream>
#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>

UtilityNPC::UtilityNPC(World * pWorld) : BaseNPC(pWorld)
{
	m_NPCJob = Child;
	AssignNPCJob(Child);
	//Get Water
	m_waterValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_waterValue.setMinMaxValues(5, 20);
	m_waterValue.setValue(getWaterValue());
	UtilityScore* pWaterSource = new UtilityScore();
	pWaterSource->addUtilityValue(&m_waterValue, 1.0f);
	m_pUtilityScoreMap["collectWater"] = pWaterSource;
	//Get Food
	m_foodValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_foodValue.setMinMaxValues(5, 20);
	m_foodValue.setValue(getFoodValue());
	UtilityScore* pFoodSource = new UtilityScore();
	pFoodSource->addUtilityValue(&m_foodValue, 1.0f);
	m_pUtilityScoreMap["collectFood"] = pFoodSource;
	//Get Rest
	m_restValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_restValue.setMinMaxValues(5, 20);
	m_restValue.setValue(getRestValue());
	UtilityScore* pRestSource = new UtilityScore();
	pRestSource->addUtilityValue(&m_restValue, 1.0f);
	m_pUtilityScoreMap["collectRest"] = pRestSource;
	//Cut Tree
	m_cutLogValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_cutLogValue.setMinMaxValues(0, 2);
	m_cutLogValue.setValue(m_pWorld->getCurrentStockpileLogs());
	UtilityScore* pLogSource = new UtilityScore();
	pLogSource->addUtilityValue(&m_cutLogValue, 0.4f );
	m_pUtilityScoreMap["harvestLog"] = pLogSource;
	//Harvest Food
	m_harvestFoodValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_harvestFoodValue.setMinMaxValues(0, 50);
	m_harvestFoodValue.setValue(m_pWorld->getCurrentStockpileFood());
	UtilityScore* pHarvestFood = new UtilityScore();
	pHarvestFood->addUtilityValue(&m_harvestFoodValue, 1.0f);
	m_pUtilityScoreMap["harvestFood"] = pHarvestFood;
	//Build House		//NPC needs to know stockpile stock
	m_buildHouseValue.setNormalizationType(UtilityValue::LINEAR);
	m_buildHouseValue.setMinMaxValues(0, 1);
	m_buildHouseValue.setValue(getNumberOfLogs() + m_pWorld->getCurrentStockpileLogs());
	UtilityScore* pBuildHouse = new UtilityScore();
	pBuildHouse->addUtilityValue(&m_buildHouseValue, 0.45f);
	m_pUtilityScoreMap["buildHouse"] = pBuildHouse;	
	//Wander Around
	m_wanderValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_wanderValue.setMinMaxValues(0, 1);
	m_wanderValue.setValue(0.5f);
	UtilityScore* pWanderSource = new UtilityScore();
	pWanderSource->addUtilityValue(&m_wanderValue, 1.f);
	m_pUtilityScoreMap["wander"] = pWanderSource;

	//m_wanderJobValue.setNormalizationType(UtilityValue::LINEAR);
	//m_wanderJobValue.setMinMaxValues(0, 1);
	//m_wanderJobValue.setValue(1.f);
	//pWanderSource->addUtilityValue(&m_wanderJobValue, 0.5f);
}

UtilityNPC::~UtilityNPC()
{

}

void UtilityNPC::selectAction(float a_fdeltaTime)
{
	//
	m_waterValue.setValue(getWaterValue());
	m_foodValue.setValue(getFoodValue());
	m_restValue.setValue(getRestValue());
	m_cutLogValue.setValue(m_pWorld->getCurrentStockpileLogs());
	m_buildHouseValue.setValue(getNumberOfLogs() + m_pWorld->getCurrentStockpileLogs());
	m_harvestFoodValue.setValue(m_pWorld->getCurrentStockpileFood());
	m_wanderValue.setValue(0.5f);
	m_pUtilityScoreMap["wander"]->setModifier(0, 0.5f);


	float fCurrentTime = (float)glfwGetTime();
	//if (fCurrentTime >= m_fLastReportTime + m_fReportTime)
	//{
	//	system("cls");
	//	std::cout << "SP Food" << m_pWorld->getCurrentStockpileFood() << "/" << m_pWorld->getMaxStockpileFood();
	//	std::cout << "SP Logs" << m_pWorld->getCurrentStockpileLogs() << "/" << m_pWorld->getMaxStockpileLogs() << std::endl;
	//}

	//Choose Best Action
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
		//float fCurrentTime = (float)glfwGetTime();
		//if (fCurrentTime >= m_fLastReportTime + m_fReportTime)
		//{
		//	std::cout << score.first.c_str() << " " << fThisScore << std::endl;
		//}
	}

	//Do Best Action
	if (strBestAction == "collectWater")
	{
		collectWater(a_fdeltaTime);
	}
	else if (strBestAction == "collectFood")
	{
		collectFood(a_fdeltaTime);
	}
	else if (strBestAction == "collectRest")
	{
		rest(a_fdeltaTime);
	}
	else if (strBestAction == "harvestLog")
	{
		harvestTree(a_fdeltaTime);
	}
	else if (strBestAction == "harvestFood")
	{
		harvestFood(a_fdeltaTime);
	}
	else if (strBestAction == "buildHouse")
	{

		buildHouse(a_fdeltaTime);
	}
	else if (strBestAction == "wander")
	{
		wander(a_fdeltaTime);
	}
	
	//std::cout << strBestAction.c_str << std::endl;


}
