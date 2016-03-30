#pragma once
#include "BaseNPC.h"
#include "UtilityMaths.h"
#include "UtilityScore.h"
#include "UtilityValue.h"
#include <map>

using namespace UtilitySystem;

class UtilityNPC :	public BaseNPC
{
public:
	UtilityNPC(World* pWorld);
	~UtilityNPC();

protected:
	void selectAction(float a_fdeltaTime) override;

private:
	std::map<std::string, UtilityScore*> m_pUtilityScoreMap;

	UtilityValue m_waterValue;
	UtilityValue m_waterDistValue;
	UtilityValue m_foodValue;
	UtilityValue m_foodDistValue;
	UtilityValue m_restValue;
	UtilityValue m_restDistValue;
	UtilityValue m_cutLogValue;
	UtilityValue m_depositLogValue;
	UtilityValue m_collectLogValue;
	UtilityValue m_buildHouseValue;

	
	
	
};

