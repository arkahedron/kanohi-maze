#include "ArchivesState.h"
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
#include "Visuals.h"

#include "StateMachineCore.h"

using namespace std;


ArchivesState::ArchivesState(StateMachineCore* pOwner)
	: m_pOwner(pOwner)
	, m_gameEnd(false)
	, m_levelName("Entry.txt")
	, m_levelsCleared(0)
{
	m_player = Player::GetInstance();
	m_level = Level::GetInstance();
}

bool ArchivesState::Load()
{
	m_levelName = m_level->SelectNewLevel();
	int* playerXpos = m_player->m_WorldActor.GetXPositionPointer();
	int* playerYpos = m_player->m_WorldActor.GetYPositionPointer();
	m_player->exited = false;
	return m_level->Load(m_levelName, playerXpos, playerYpos);
}

void ArchivesState::Enter()
{
	m_level->m_levelsCleared = 0;
	Load();
}

bool ArchivesState::Update(bool processInput)
{
	Draw();

	if (m_player->HandleMovement())
	{ 
		m_level->ClearLevel();
		Load();
	}

	m_gameEnd = m_player->exited;

	if (m_gameEnd) 
	{ 
		m_level->ClearLevel();
		system("cls");
		cout << endl << " Exiting maze..." << endl;
		m_pOwner->LoadScene(StateMachineCore::SceneName::MainMenu);

		return false; 
	}
	
	return false;
}

void ArchivesState::Draw() 
{
	m_level->Draw();
	

	short int heightBuffer = (int)m_level->m_height + 5;
	m_visuals.ResetCursor({ (0),(heightBuffer) });
}