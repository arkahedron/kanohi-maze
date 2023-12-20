#pragma once
//#include "Level.h"

#include "Input.h"
#include "Visuals.h"
#include "Randomizer.h"
#include <string>

#include "GameStateMachine.h"

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

	GameStateMachine* m_pStateMachine;
public:
	Game();
	~Game();

	void Initialize(GameStateMachine* pStateMachine);
	void RunGameLoop();
	void Deinitialize();
	bool Update(bool processInput = true);


	bool lvlDrawn;
	bool levelEnd;
	bool exitedGame;

	bool Load();
	void Run();

private:

};