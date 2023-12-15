#pragma once
#include "Randomizer.h"
#include "Visuals.h"
#include <string>
#include <vector>

class WorldActor;

class Level
{
	Visuals m_visuals;
	Randomizer m_randomizer;
	std::string m_levelToLoad;
	std::string m_defaultLevel;
	char* m_pLevelData;

	std::vector<WorldActor*> m_pActors;

protected:
	Level();
	~Level();
	static Level* instance;
public:
	static Level* GetInstance();

	int m_height;
	int m_width;
	int m_holeX;
	int m_holeY;
	int m_levelsCleared;

	std::string SelectNewLevel();

	bool Load(std::string levelName, int* playerX, int* playerY);

	void SetDrawnState(bool isDrawn) { m_levelDrawn = isDrawn; };
	bool GetDrawnState() { return m_levelDrawn; };

	char GetSpaceAtPosition(int x, int y);
	bool IsSpace(int x, int y);
	bool IsGoal(int x, int y);

	void ClearSpace(int x, int y);
	void ClearLevel();

	void Draw();

	WorldActor* UpdateActors(int x, int y);
	WorldActor* GetActorAtPos(int x, int y);

private:
	bool m_levelDrawn;

	bool Convert(int* playerX, int* playerY);
	int GetIndexFromCoordinates(int x, int y);

};

