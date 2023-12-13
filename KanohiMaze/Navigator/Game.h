#pragma once
//#include "Level.h"

#include "Input.h"
#include "Visuals.h"
#include "Randomizer.h"
#include <string>

class Player;
class Level;

class Game
{
	Level* m_level;
	Player* m_player;
	Input m_input;
	Visuals m_visuals;
	Randomizer m_randomizer;

	std::string m_levelName;

public:
	Game();
	~Game();

	//Level GetLevelRef() { return m_level; };
	//Player* GetPlayerRef() { return m_player; };

	int roomsCleared;
	bool lvlDrawn;
	bool levelEnd;
	bool exitedGame;

	bool Load();
	void Run();
	void Unload();

	//void ChestLoot();

	//int GetPlayerLives() { return m_player->lives; }

private:
	//bool Update();
	//void Draw();
	//void Interact(int x, int y);

	//bool HandleCollision(int newPlayerX, int newPlayerY);
};