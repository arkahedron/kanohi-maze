#pragma once
#include "Randomizer.h"
#include <string>


class Level
{
	Randomizer m_randomizer;
	std::string m_levelToLoad;
	std::string m_defaultLevel;
	char* m_pLevelData;

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
	void Draw(int x, int y);

	bool IsSpace(int x, int y);
	bool IsDoor(int x, int y);
	bool IsKey(int x, int y);
	bool IsMat(int x, int y);
	bool IsBox(int x, int y);
	bool IsGoal(int x, int y);

	void ClearSpace(int x, int y);
	void ClearLevel();

	char GetSpaceAtPosition(int x, int y);

private:
	int GetIndexFromCoordinates(int x, int y);
	bool Convert(int* playerX, int* playerY);

};