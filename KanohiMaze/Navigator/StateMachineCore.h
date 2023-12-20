#pragma once
#include "GameStateMachine.h"

class Game;
class GameStates;




class StateMachineCore : public GameStateMachine
{
public:
	enum class SceneName
	{
		None,
		MainMenu,
		Foundry,
		Archives
	};

private:
	Game* m_pOwner;

	GameState* m_pCurrentState;
	GameState* m_pNewState;

public:
	StateMachineCore(Game* pOwner);

	virtual bool Init() override;
	virtual bool UpdateCurrentState(bool processInput = true) override;
	virtual void DrawCurrentState() override;
	virtual void ChangeState(GameState* pNewState) override;
	virtual void CleanUp() override;
	void LoadScene(SceneName scene);

};

