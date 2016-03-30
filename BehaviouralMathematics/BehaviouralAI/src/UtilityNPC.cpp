#include "UtilityNPC.h"
#include "World.h"
#include <iostream>
#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>

UtilityNPC::UtilityNPC(World * pWorld) : BaseNPC(pWorld)
{
	//Get Water
	m_waterValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_waterValue.setMinMaxValues(5, 20);
	m_waterValue.setValue(getWaterValue());
	UtilityScore* pWaterSource = new UtilityScore();
	pWaterSource->addUtilityValue(&m_waterValue, 1.0f);
	m_pUtilityScoreMap["collectWater"] = pWaterSource;
	//Get WaterDistance
	//m_waterDistValue.setNormalizationType(UtilityValue::LINEAR);
	//m_waterDistValue.setMinMaxValues(10, 40);
	//m_waterDistValue.setValue(glm::distance(getPosition(), m_pWorld->FindClosestWater(getPosition())));
	//pWaterSource->addUtilityValue(&m_waterDistValue, 1.0f);
	//Get Food
	m_foodValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_foodValue.setMinMaxValues(5, 20);
	m_foodValue.setValue(getFoodValue());
	UtilityScore* pFoodSource = new UtilityScore();
	pFoodSource->addUtilityValue(&m_foodValue, 1.0f);
	m_pUtilityScoreMap["collectFood"] = pFoodSource;
	//Get FoodDistance
	//m_foodDistValue.setNormalizationType(UtilityValue::LINEAR);
	//m_foodDistValue.setMinMaxValues(-10, 25);
	//m_foodDistValue.setValue(glm::distance(getPosition(), m_pWorld->FindClosestFood(getPosition())));
	//pFoodSource->addUtilityValue(&m_foodDistValue, 1.f);
	//Get Rest
	m_restValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_restValue.setMinMaxValues(5, 20);
	m_restValue.setValue(getRestValue());
	UtilityScore* pRestSource = new UtilityScore();
	pRestSource->addUtilityValue(&m_restValue, 1.0f);
	m_pUtilityScoreMap["collectRest"] = pRestSource;
	//Get RestDistance
	//m_restDistValue.setNormalizationType(UtilityValue::LINEAR);
	//m_restDistValue.setMinMaxValues(10, 40);
	//m_restDistValue.setValue(glm::distance(getPosition(), m_pWorld->FindClosestRest(getPosition())));
	//pRestSource->addUtilityValue(&m_restDistValue, 1.0f);
	//Cut Tree
	m_cutLogValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_cutLogValue.setMinMaxValues(0, 2);
	m_cutLogValue.setValue(m_pWorld->getCurrentStockpileLogs());
	UtilityScore* pLogSource = new UtilityScore();
	pLogSource->addUtilityValue(&m_cutLogValue, 0.4f );
	m_pUtilityScoreMap["harvestLog"] = pLogSource;
	//Take Log to Stockpile
	//m_depositLogValue.setNormalizationType(UtilityValue::LINEAR);
	//m_depositLogValue.setMinMaxValues(0, 2);
	//m_depositLogValue.setValue(getRawLogs());
	//UtilityScore* pLogToStockpile = new UtilityScore();
	//pLogToStockpile->addUtilityValue(&m_depositLogValue, 0.5f);
	//m_pUtilityScoreMap["depositLog"] = pLogToStockpile;
	//Collect Log from Stockpile
	m_collectLogValue.setNormalizationType(UtilityValue::LINEAR);
	m_collectLogValue.setMinMaxValues(0, 1);
	m_collectLogValue.setValue(m_pWorld->getCurrentStockpileLogs());
	UtilityScore* pCollectLog = new UtilityScore();
	pCollectLog->addUtilityValue(&m_collectLogValue, 0.5f);
	m_pUtilityScoreMap["collectLog"] = pCollectLog;

	//Build House		//NPC needs to know stockpile stock
	m_buildHouseValue.setNormalizationType(UtilityValue::LINEAR);
	m_buildHouseValue.setMinMaxValues(0, 1);
	m_buildHouseValue.setValue(m_pWorld->getCurrentStockpileLogs());
	UtilityScore* pBuildHouse = new UtilityScore();
	pBuildHouse->addUtilityValue(&m_buildHouseValue, 0.45f);
	m_pUtilityScoreMap["buildHouse"] = pBuildHouse;	
}

UtilityNPC::~UtilityNPC()
{

}

void UtilityNPC::selectAction(float a_fdeltaTime)
{
	//
	m_waterValue.setValue(getWaterValue());
	m_foodValue.setValue(getFoodValue());
	//m_foodDistValue.setValue(glm::distance(getPosition(), m_pWorld->FindClosestFood(getPosition())));
	m_restValue.setValue(getRestValue());
	//m_restDistValue.setValue(glm::distance(getPosition(), m_pWorld->FindClosestFood(getPosition())));
	m_cutLogValue.setValue(getRawLogs());
	//m_depositLogValue.setValue(getNumberOfLogs());
	m_collectLogValue.setValue(m_pWorld->getCurrentStockpileLogs());
	m_buildHouseValue.setValue(getNumberOfLogs());


	float fCurrentTime = (float)glfwGetTime();
	if (fCurrentTime >= m_fLastReportTime + m_fReportTime)
	{
		system("cls");
	}

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
		if (fCurrentTime >= m_fLastReportTime + m_fReportTime)
		{
			std::cout << score.first.c_str() << " " << fThisScore << std::endl;
		}
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
	//else if (strBestAction == "depositLog")
	//{
	//	depositStockpileLog(a_fdeltaTime);
	//}
	else if (strBestAction == "collectLog")
	{
		collectStockpileLog(a_fdeltaTime);
	}
	else if (strBestAction == "buildHouse")
	{

		buildHouse(a_fdeltaTime);
	}
	
	//std::cout << strBestAction.c_str << std::endl;


}
