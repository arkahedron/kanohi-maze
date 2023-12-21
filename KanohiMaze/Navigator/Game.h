#pragma once

#include "GameStateMachine.h"

class Player;
class Level;

class Game
{
	Level* m_level;
	Player* m_player;
	GameStateMachine* m_pStateMachine;

public:
	Game();
	~Game();

	void Initialize(GameStateMachine* pStateMachine);
	void RunGameLoop();
	void Deinitialize();
	bool Update(bool processInput = true);

};