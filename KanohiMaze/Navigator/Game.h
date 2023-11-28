#pragma once
#include "Level.h"
#include "Player.h"
#include "Input.h"
#include "Visuals.h"
#include "Randomizer.h"
#include <string>


class Game
{
	Level m_level;
	Player m_player;
	Input m_input;
	Visuals m_visuals;
	Randomizer m_randomizer;
	std::string m_levelName;

public:
	Game();
	~Game();

	Level GetLevelRef() { return m_level; };
	Player GetPlayerRef() { return m_player; };

	bool lvlDrawn;
	bool levelEnd;
	bool exitedGame;

	bool Load();
	void Run();
	void Unload();

	void ChestLoot();

private:
	bool Update();
	void Draw();
	void Interact(int x, int y);
};