#ifndef WORLD_H_
#define WORLD_H_

#include "glm/glm.hpp"
#include <list>
#include <vector>

class UtilityNPC;
class WorldObject;
class House;
class Food;
class Water;
class Rest;
class Tree;

class World
{
public:
	World();
	~World();

	void update(float a_fdeltaTime);
	void render();

	void addLogToHouse();
	unsigned int getHouseLogsRequired() const { return m_uiHouseLogsRequired;  }
	unsigned int getCurrentHouseLogs() const { return m_uiHouseCurrentLogs; }
	bool isHouseBuilt() { return m_uiHouseCurrentLogs == m_uiHouseLogsRequired;  }

	float addLogsToStockpile(unsigned int logs); //Returns = logs that weren't deposited (0 if all fit)
	float removeLogsFromStockpile(unsigned int logs); //Return = logs that were withdrawn (0 if was empty)
	float addFoodToStockpile(unsigned int amount);
	float removeFoodFromStockpile(unsigned int amount);
	unsigned int getCurrentStockpileLogs() const { return m_uiStockpileCurrentLogs; }
	unsigned int getMaxStockpileLogs() const { return m_uiStockpileMaxLogs; }
	unsigned int getCurrentStockpileFood() const { return m_uiStockpileCurrentFood; }
	unsigned int getMaxStockpileFood() const { return m_uiStockpileMaxFood; }

	std::vector<UtilityNPC*> NPCVector;
	std::list<WorldObject*> worldObjectList;
	std::vector<House*> houseVector;
	std::vector<Food*> foodVector;
	std::vector<Water*> waterVector;
	std::vector<Rest*> restVector;
	std::vector<Tree*> treeVector;

	glm::vec3 getHouseLocation() const;
	glm::vec3 getTreeLocation() const;
	glm::vec3 getWaterLocation() const;
	glm::vec3 getFoodLocation() const;
	glm::vec3 getRestedLocation() const;
	glm::vec3 getStockpileLocation() const;

	bool interactWithFood();
	bool interactWithWater();
	bool interactWithRested();

	bool interactWithTree();
	bool interactWithHouse();
	bool interactWithStockpile();

	void BuildNewFood(glm::vec3 a_location);
	void BuildNewWater(glm::vec3 a_location);
	void BuildNewRest(glm::vec3 a_location);
	void BuildNewTree(glm::vec3 a_location);
	void BuildNewHouse(glm::vec3 a_location);
	
	glm::vec3 FindClosestFood(glm::vec3 a_myLoc);
	glm::vec3 FindClosestWater(glm::vec3 a_myLoc);
	glm::vec3 FindClosestRest(glm::vec3 a_myLoc);
	glm::vec3 FindClosestTree(glm::vec3 a_myLoc);

private:
	unsigned int m_uiHouseLogsRequired;
	unsigned int m_uiHouseCurrentLogs;
	unsigned int m_uiStockpileCurrentLogs;
	unsigned int m_uiStockpileMaxLogs;
	unsigned int m_uiStockpileCurrentFood;
	unsigned int m_uiStockpileMaxFood;

	glm::vec3 m_houseLocation;
	glm::vec3 m_treeLocation;
	glm::vec3 m_stockpileLocation;

	glm::vec3 m_waterLocation;
	glm::vec3 m_foodLocation;
	glm::vec3 m_restedLocation;

	float m_fHouseInteractTime;
	float m_fTreeInteractTime;
	float m_fFoodInteractTime;
	float m_fWaterInteractTime;
	float m_fRestedInteractTime;
	float m_fStockpileInteractTime;

	float m_fLastHouseInteractTime;
	float m_fLastTreeInteractTime;
	float m_fLastWaterInteractTime;
	float m_fLastFoodInteractTime;
	float m_fLastRestedInteractTime;
	float m_fLastStockpileInteractTime;
};

#endif // !WORLD_H_
