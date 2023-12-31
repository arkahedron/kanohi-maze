#pragma once
#include "GameState.h"
#include "Visuals.h"
#include "Input.h"

#include <string>

class StateMachineCore;
class SaveManager;

class MainMenuState : public GameState
{
	Visuals m_visuals;
	Input m_input;
	StateMachineCore* m_pOwner;

public:
	MainMenuState(StateMachineCore* pOwner);
	~MainMenuState() = default;

	virtual bool Update(bool processInput = true) override;
	virtual void Draw() override;

	int MenuSelector();

};

