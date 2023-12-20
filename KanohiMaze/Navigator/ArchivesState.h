#pragma once
#include <string>
#include "GameState.h"
#include "Level.h"
#include "Player.h"
#include "Visuals.h"

class StateMachineCore;

class ArchivesState : public GameState
{
	StateMachineCore* m_pOwner;

	Level* m_level;
	Player* m_player;
	Visuals m_visuals;

	bool m_gameEnd;
	int m_levelsCleared;
	std::string m_levelName;

public:
	ArchivesState(StateMachineCore* pOwner);
	virtual void Enter() override;
	virtual bool Update(bool processInput = true) override;
	virtual void Draw() override;

private:
	bool Load();
};

