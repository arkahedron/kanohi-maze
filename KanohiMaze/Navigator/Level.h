#pragma once
#include "Randomizer.h"
#include "Visuals.h"
#include <string>
#include <vector>

class PlacableActor;

class Level
{
	Visuals m_visuals;
	Randomizer m_randomizer;
	std::string m_levelToLoad;
	std::string m_defaultLevel;
	char* m_pLevelData;

	std::vector<PlacableActor*> m_pActors;

public:
	Level();
	~Level();

	int m_height;
	int m_width;
	int m_holeX;
	int m_holeY;
	int m_levelsCleared;

	std::string SelectNewLevel();

	bool Load(std::string levelName, int* playerX, int* playerY);
	void Draw(bool drawn = false);
	//void Draw(int x, int y);

	PlacableActor* UpdateActors(int x, int y);
	PlacableActor* GetActorAtPos(int x, int y);

	bool IsSpace(int x, int y);
	/*bool IsWall(int x, int y);

	bool IsDoor(int x, int y);
	bool IsKey(int x, int y);
	bool IsMat(int x, int y);
	bool IsBox(int x, int y);*/
	bool IsGoal(int x, int y);

	void ClearSpace(int x, int y);
	void ClearLevel();

	char GetSpaceAtPosition(int x, int y);

private:
	bool Convert(int* playerX, int* playerY);
	int GetIndexFromCoordinates(int x, int y);

};

