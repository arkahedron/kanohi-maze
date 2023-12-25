#pragma once
#include "Randomizer.h"
#include "Visuals.h"
#include <string>
#include <vector>

using namespace std;

class WorldActor;

class Level
{
	Visuals m_visuals;
	Randomizer m_randomizer;
	string m_levelToLoad;
	string m_defaultLevel;
	char* m_pLevelData;

	vector<WorldActor*> m_pActors;

protected:
	Level();
	~Level();
	
public:
	static Level* GetInstance();
	void DestroyLevel() { delete instance; instance = nullptr; }

	int m_height;
	int m_width;
	int m_holeX;
	int m_holeY;
	int m_levelsCleared;

	string SelectNewLevel();

	bool Load(string levelName, int* playerX, int* playerY);

	void SetDrawnState(bool isDrawn);

	char GetSpaceAtPosition(int x, int y);

	void ClearSpace(int x, int y, WorldActor* delActor = nullptr);
	void ClearLevel();

	void Draw();

	WorldActor* GetActorAtPos(int x, int y);

	bool edgeHit;
	COORD edgeShiftBuffer;
	COORD minCorner;
	COORD maxCorner;
private:
	static Level* instance;
	bool m_levelDrawn;

	bool Convert(int* playerX, int* playerY);

	void AlignScreenToPlayer(int* playerX, int* playerY);
	int GetIndexFromCoordinates(int x, int y);
	void CleanUpScreen();
	bool HandlePlayerEdgeHit();
};

