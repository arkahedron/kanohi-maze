#include "Game.h"

#include "GameStateMachine.h"
#include "Level.h"
#include "Player.h"


using namespace std;


Game::Game()
	: m_pStateMachine(nullptr)
{
	m_player = Player::GetInstance();
	m_level = Level::GetInstance();
}
Game::~Game()
{

}

void Game::Initialize(GameStateMachine* pStateMachine)
{
	if(pStateMachine)
	{
		pStateMachine->Init();
		m_pStateMachine = pStateMachine;
	}
}
void Game::RunGameLoop()
{
	bool isGameOver = false;

	while (!isGameOver)
	{
		m_pStateMachine->DrawCurrentState();

		isGameOver = Update();
	}
}
void Game::Deinitialize()
{
	if(m_pStateMachine)
	{
		m_pStateMachine->CleanUp();
	}
}
bool Game::Update(bool processInput)
{
	return m_pStateMachine->UpdateCurrentState(processInput);
}
