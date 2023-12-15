#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>

#include "Level.h"
#include "Player.h"
#include "Enemy.h"
#include "Key.h"
#include "Door.h"
#include "Goal.h"
#include "Chest.h"

#include <assert.h>

using namespace std;

Game::Game()
	: levelEnd(false)
	, exitedGame(false)
	, m_levelName("Entry.txt")
	, m_visuals()
	, lvlDrawn(false)
	, roomsCleared(0)
{
	m_player = Player::GetInstance();
	m_level = Level::GetInstance();
}
Game::~Game()
{

}

bool Game::Load()
{
	m_levelName = m_level->SelectNewLevel();
	int* playerXpos = m_player->m_WorldActor.GetXPositionPointer();
	int* playerYpos = m_player->m_WorldActor.GetYPositionPointer();
	return m_level->Load(m_levelName, playerXpos, playerYpos);
}
void Game::Run()
{
	m_level->Draw();
	m_player->m_WorldActor.Draw();

	short int heightBuffer = (int)m_level->m_height + 5;
	m_visuals.ResetCursor({ (0),(heightBuffer) });

	levelEnd = m_player->HandleMovement();
	exitedGame = m_player->exited;

	if (levelEnd) { cout << endl << "EXITING MAZE"; }
}
