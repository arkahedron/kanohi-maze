#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

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
	//m_visuals.SetLevelRef(&m_level);
	//m_player->SetPlayerRef(&m_player);
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
void Game::Unload()
{

}

/*POLYMORPHIC*/
//bool Game::HandleCollision(int newPlayerX, int newPlayerY)
//{
//	WorldActor* collidedActor = m_level->UpdateActors(newPlayerX, newPlayerY);
//	if (collidedActor != nullptr && collidedActor->IsActive())
//	{
//		switch (collidedActor->GetType())
//		{
//		case ActorType::Enemy:
//		{
//			Enemy* collidedEnemy = dynamic_cast<Enemy*>(collidedActor);
//			assert(collidedEnemy);
//			collidedEnemy->Remove();
//			m_player->SetPosition(newPlayerX, newPlayerY);
//			m_player->DecrementLives();
//			if(m_player->GetLives() < 0) { return true;}
//			break;
//		}
//		case ActorType::Goal:
//		{
//			Goal* collidedGoal = dynamic_cast<Goal*>(collidedActor);
//			assert(collidedGoal);
//			m_level->ClearLevel();
//			roomsCleared++;
//			lvlDrawn = false;
//			return true;
//		}
//		default:
//			break;
//		}
//	}
//	else if (m_level->IsSpace(newPlayerX, newPlayerY))
//	{
//		m_player->SetPosition(newPlayerX, newPlayerY);
//	}
//	else if (m_level->IsWall(newPlayerX, newPlayerY))
//	{ /*hit wall, do nothing*/ }
//	return false;
//}

//
//void Game::Draw()
//{
//	if (!lvlDrawn)
//	{
//		m_visuals.ResetCursor(); /*required for correct map placement*/
//		m_visuals.DrawTop();
//		for (int y = 0; y < m_level->m_height; y++)
//		{
//			m_visuals.DrawLeft(y);
//			for (int x = 0; x < m_level->m_width; x++)
//			{
//				//Actual Per-Space print
//				if (m_player->GetXPosition() == x && m_player->GetYPosition() == y) 
//				{
//					cout << m_player->GoodDraw();/*old player draw*/
//				}
//				else
//				{
//					//Colorize other unique symbols
//					if (m_level->IsDoor(x, y)) {
//						if (m_player->HasKey()) { m_visuals.ColorText(AColor::DarkGreen); }
//						else { m_visuals.ColorText(AColor::DarkRed); }
//					}
//					else if (m_level->IsKey(x, y)) { m_visuals.ColorText(AColor::Yellow); }
//					else if (m_level->IsMat(x, y)) { m_visuals.ColorText(AColor::Grey); }
//					else if (m_level->IsBox(x, y)) { m_visuals.ColorText(AColor::Orange); }
//					else if (m_level->IsGoal(x, y)) { m_visuals.ColorText(AColor::White); }
//					else { m_visuals.ResetTextColor(); }
//					//Print relevant symbol and reset color to base
//					m_level->Draw(x, y);
//					m_visuals.ResetTextColor();
//				}
//			}
//			m_visuals.DrawRight(y);
//		}
//		m_visuals.DrawBottom();
//		m_visuals.DrawMazeControls();
//	}
//	lvlDrawn = true;
//}

//void Game::Interact(int x, int y)
//{
//	int actX = x;
//	int actY = y;
//	switch (m_player->playerFacing)
//	{
//	case Direction::Up:
//	{ actY--; break; }
//	case Direction::Down:
//	{ actY++; break; }
//	case Direction::Left:
//	{ actX--; break; }
//	case Direction::Right:
//	{ actX++; break; }
//	default: break;
//	}
//	//Interact with space player is facing
//	WorldActor* actRef =  m_level->GetActorAtPos(actX, actY);
//	if (actRef != nullptr) {
//		if (actRef->IsActive())
//		{
//			actRef->Interact();
//			/*system("cls");*/
//			lvlDrawn = false;
//		}
//	}
//}