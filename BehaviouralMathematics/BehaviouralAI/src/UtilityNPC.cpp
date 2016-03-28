#include "UtilityNPC.h"
#include "World.h"

UtilityNPC::UtilityNPC(World * pWorld) : BaseNPC(pWorld)
{
	//Get Water
	m_waterValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_waterValue.setMinMaxValues(0, 20);
	m_waterValue.setValue(getWaterValue());
	UtilityScore* pWaterSource = new UtilityScore();
	pWaterSource->addUtilityValue(&m_waterValue, 1.0f);
	m_pUtilityScoreMap["collectWater"] = pWaterSource;
	//Get Food
	m_foodValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_foodValue.setMinMaxValues(0, 20);
	m_foodValue.setValue(getFoodValue());
	UtilityScore* pFoodSource = new UtilityScore();
	pFoodSource->addUtilityValue(&m_foodValue, 1.0f);
	m_pUtilityScoreMap["collectFood"] = pFoodSource;
	//Get Rest
	m_restValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_restValue.setMinMaxValues(0, 20);
	m_restValue.setValue(getRestValue());
	UtilityScore* pRestSource = new UtilityScore();
	pRestSource->addUtilityValue(&m_restValue, 1.0f);
	m_pUtilityScoreMap["collectRest"] = pRestSource;
	//Cut Tree
	m_cutLogValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_cutLogValue.setMinMaxValues(0, 2);
	m_cutLogValue.setValue(getNumberOfHarvestedLogs());
	UtilityScore* pLogSource = new UtilityScore();
	pLogSource->addUtilityValue(&m_cutLogValue, 1.0f);
	m_pUtilityScoreMap["harvestLog"] = pLogSource;
	//Take Log to Stockpile
	m_depositLogValue.setNormalizationType(UtilityValue::LINEAR);
	m_depositLogValue.setMinMaxValues(0, 2);
	m_depositLogValue.setValue(getNumberOfHarvestedLogs());
	UtilityScore* pLogToStockpile = new UtilityScore();
	pLogToStockpile->addUtilityValue(&m_depositLogValue, 1.0f);
	m_pUtilityScoreMap["depositLog"] = pLogToStockpile;
	//Collect Log from Stockpile
	m_collectLogValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
	m_collectLogValue.setMinMaxValues(0, 1);
	m_collectLogValue.setValue(m_pWorld->getCurrentStockpileLogs());
	UtilityScore* pCollectLog = new UtilityScore();
	pCollectLog->addUtilityValue(&m_collectLogValue, 1.0f);
	m_pUtilityScoreMap["collectLog"] = pCollectLog;

	//Build House		//NPC needs to know stockpile stock
	m_buildHouseValue.setNormalizationType(UtilityValue::LINEAR);
	m_buildHouseValue.setMinMaxValues(0, 1);
	m_buildHouseValue.setValue(getNumberOfLogs());
	UtilityScore* pBuildHouse = new UtilityScore();
	pBuildHouse->addUtilityValue(&m_buildHouseValue, 1.0f);
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
	m_restValue.setValue(getRestValue());
	m_cutLogValue.setValue(getNumberOfHarvestedLogs());
	m_depositLogValue.setValue(getNumberOfHarvestedLogs());
	m_collectLogValue.setValue(m_pWorld->getCurrentStockpileLogs());
	m_buildHouseValue.setValue(getNumberOfLogs());

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
		chopTree(a_fdeltaTime);
	}
	else if (strBestAction == "depositLog")
	{
		depositStockpileLog(a_fdeltaTime);
	}
	else if (strBestAction == "collectLog")
	{
		collectStockpileLog(a_fdeltaTime);
	}
	else
	{
		buildHouse(a_fdeltaTime);
	}
	

}
