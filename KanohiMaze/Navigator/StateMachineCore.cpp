#include "StateMachineCore.h"
#include "MainMenuState.h"
//#include "FoundryState.h"
#include "ArchivesState.h"
#include "Game.h"


StateMachineCore::StateMachineCore(Game* pOwner)
	: m_pOwner(pOwner)
	, m_pCurrentState(nullptr)
	, m_pNewState(nullptr)
{

}

bool StateMachineCore::Init()
{
	LoadScene(SceneName::MainMenu);
	return true;
}
bool StateMachineCore::UpdateCurrentState(bool processInput)
{
	bool done = false;
	if (m_pNewState != nullptr)
	{
		ChangeState(m_pNewState);
		m_pNewState = nullptr;
	}

	if (m_pCurrentState != nullptr)
	{
		done = m_pCurrentState->Update(processInput);
	}
	return done;
}
void StateMachineCore::DrawCurrentState()
{
	if (m_pCurrentState)
	{
		m_pCurrentState->Draw();
	}
}
void StateMachineCore::ChangeState(GameState* pNewState)
{
	if (m_pCurrentState)
	{
		m_pCurrentState->Exit();
	}

	delete m_pCurrentState;
	m_pCurrentState = pNewState;
	pNewState->Enter();
}
void StateMachineCore::CleanUp()
{
	if (m_pCurrentState)
	{
		m_pCurrentState->Exit();
		delete m_pCurrentState;
	}
}
void StateMachineCore::LoadScene(SceneName scene)
{
	switch (scene)
	{
	case SceneName::None:
		break;
	case SceneName::MainMenu:
		m_pNewState = new MainMenuState(this);
		break;
	case SceneName::Foundry:
		//m_pNewState = new FoundryState(this);
		break;
	case SceneName::Archives:
		m_pNewState = new ArchivesState(this);
		break;
	default:
		break;
	}
}