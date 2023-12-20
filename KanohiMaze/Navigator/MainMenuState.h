#pragma once
#include "GameState.h"
#include "Visuals.h"

class StateMachineCore;

class MainMenuState : public GameState
{
	Visuals m_visuals;
	StateMachineCore* m_pOwner;

public:
	MainMenuState(StateMachineCore* pOwner);
	~MainMenuState() = default;

	virtual bool Update(bool processInput = true) override;
	virtual void Draw() override;
};

