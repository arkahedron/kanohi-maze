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
	return m_level->Load(m_levelName, m_player->m_WorldActor.GetXPositionPointer(), m_player->m_WorldActor.GetYPositionPointer());
}
void Game::Run()
{
	Draw();
	levelEnd = Update();
	exitedGame = m_player->exited;

	if (levelEnd)
	{
		cout << endl << "EXITING MAZE";
	}
}
void Game::Unload()
{

}


bool Game::Update()
{
	char input = (char)_getch();
	int newPlayerX = m_player->m_WorldActor.GetXPosition();
	int newPlayerY = m_player->m_WorldActor.GetYPosition();

	//Actor the player might be overlapping, validate it when using it
	WorldActor* ingressActor;
	
	if (input == 'e' || input == 'E')
	{
		Interact(newPlayerX, newPlayerY);
	}

	//Restoring correct symbol on space after player moves from it
	ingressActor = m_level->GetActorAtPos(newPlayerX, newPlayerY);
	if (ingressActor != nullptr) 
	{ ingressActor->Draw(); }
	else 
	{ m_visuals.DrawAtSpace(newPlayerX, newPlayerY, ' '); }
	


	//Movement inputs
	switch (input) 
	{
	case '\t':
		m_player->OpenMenu();
		lvlDrawn = false;
		break;
	case 'w': newPlayerY--;
	case 'W':
	{ m_player->SetFacingDirection(Direction::Up); break; }
	case 's': newPlayerY++;
	case 'S':
	{ m_player->SetFacingDirection(Direction::Down); break; }
	case 'a': newPlayerX--;
	case 'A':
	{ m_player->SetFacingDirection(Direction::Left); break; }
	case 'd': newPlayerX++;
	case 'D':
	{ m_player->SetFacingDirection(Direction::Right); break; }
	default: break;
	}

	//Move player and check for unique space
	bool pathableSpace = false;
	
	ingressActor = m_level->GetActorAtPos(newPlayerX, newPlayerY);

	if (m_level->IsSpace(newPlayerX, newPlayerY))
	{
		if (ingressActor != nullptr) {
			bool isImpassable = ingressActor->GetSolidity();
				if(!isImpassable)
				{
					pathableSpace = true;
				} 
			} else { pathableSpace = true; }
	}else { pathableSpace = false; }

	if (pathableSpace)
	{
		//Confine player to level edges
		if (newPlayerX < 0)
			newPlayerX = 0;
		else if (newPlayerX == m_level->m_width)
			newPlayerX = m_level->m_width - 1;
		if (newPlayerY < 0)
			newPlayerY = 0;
		else if (newPlayerY == m_level->m_height)
			newPlayerY = m_level->m_height - 1;
		m_player->m_WorldActor.SetPosition(newPlayerX, newPlayerY);
	}
	else if (m_level->IsGoal(newPlayerX, newPlayerY))
	{
		m_level->ClearSpace(newPlayerX, newPlayerY);
		m_player->m_WorldActor.SetPosition(newPlayerX, newPlayerY);
		m_level->ClearLevel();
		roomsCleared++;
		lvlDrawn = false;
		return true;
	}
	else {}
	//m_player->Draw();
	//m_visuals.DrawAtSpace(m_player->GetXPosition(), m_player->GetYPosition(), m_player->GoodDraw());
	m_visuals.ResetTextColor();

	if (newPlayerX == m_player->m_WorldActor.GetXPosition() && newPlayerY == m_player->m_WorldActor.GetYPosition())
	{
		return false;
	}
	//else
	//{
	//	return HandleCollision(newPlayerX, newPlayerY);
	//}

	return false;
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


void Game::Draw()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	//system("cls");

	m_level->Draw(lvlDrawn);

	lvlDrawn = true;
	COORD actorCursorPosition;
	actorCursorPosition.X = m_player->m_WorldActor.GetXPosition();
	actorCursorPosition.Y = m_player->m_WorldActor.GetYPosition();
	SetConsoleCursorPosition(console, actorCursorPosition);
	m_player->m_WorldActor.Draw();
	
	COORD currentCursorPosition;
	currentCursorPosition.X = 0;
	currentCursorPosition.Y = m_level->m_height + 5;
	SetConsoleCursorPosition(console, currentCursorPosition);
}

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

void Game::Interact(int x, int y)
{
	int actX = x;
	int actY = y;
	switch (m_player->playerFacing)
	{
	case Direction::Up:
	{ actY--; break; }
	case Direction::Down:
	{ actY++; break; }
	case Direction::Left:
	{ actX--; break; }
	case Direction::Right:
	{ actX++; break; }
	default: break;
	}

	//Interact with space player is facing
	char interactedWith = m_level->GetSpaceAtPosition(actX, actY);

	WorldActor* actRef =  m_level->GetActorAtPos(actX, actY);
	if (actRef != nullptr) {
		ActorType actTypeRef = actRef->GetType();
	
		switch (actTypeRef)
		{
		case ActorType::Door:
			if (m_player->HasKey())
			{
				if (m_input.BinaryChoice("USE KEY ON DOOR?")) {
					actRef->Remove();
					m_level->ClearSpace(actX, actY);
					m_player->UseKey();
					///PlayDoorOpenEffect();
					cout << '\r' << " [DOOR OPENED]" << endl;
					system("pause");
				}
			}
			else
			{
				///PlayDoorClosedEffect();
				cout << '\r' << " [DOOR LOCKED]" << endl;
				system("pause");
			}
			system("cls");
			lvlDrawn = false;
			break;
		case ActorType::Enemy:
			break;
		case ActorType::Goal:
			break;
		case ActorType::Key:
			if (m_input.BinaryChoice("COLLECT KEY?")) {
				m_player->PickupKey(1);
				actRef->Remove();
				m_level->ClearSpace(actX, actY);
				///PlayPickupEffect();
				//m_visuals.SubText("KEY COLLECTED");

			}
			else {}
			system("cls");
			lvlDrawn = false;
			break;
		case ActorType::Chest:
			if (m_input.BinaryChoice("SEARCH CHEST?")) {
				//random items?
				ChestLoot();
				///PlayPickupEffect();
				actRef->Remove();
				m_level->ClearSpace(actX, actY);
			}
			else {}
			system("cls");
			lvlDrawn = false;
			break;
		case ActorType::Ore:
			if (m_input.BinaryChoice("COLLECT ORE?")) {
				m_player->PickupMat(1);
				actRef->Remove();
				m_level->ClearSpace(actX, actY);
				///PlayPickupEffect();
				//m_visuals.SubText("ORE COLLECTED");
			}
			else {}
			system("cls");
			lvlDrawn = false;
			break;
		case ActorType::Player:
			break;
		default:
			break;
		}
	}



	//if (m_level->IsKey(actX, actY))
	//{
	//	if (m_input.BinaryChoice("COLLECT KEY?")) {
	//		m_player->PickupKey(1);
	//		m_level->ClearSpace(actX, actY);
	//		///PlayPickupEffect();
	//		//m_visuals.SubText("KEY COLLECTED");
	//	}
	//	else {}
	//	system("cls");
	//	lvlDrawn = false;
	//}
	//else if (m_level->IsMat(actX, actY))
	//{
	//	if (m_input.BinaryChoice("COLLECT ORE?")) {
	//		m_player->PickupMat(1);
	//		m_level->ClearSpace(actX, actY);
	//		///PlayPickupEffect();
	//		//m_visuals.SubText("ORE COLLECTED");
	//	}
	//	else {}
	//	system("cls");
	//	lvlDrawn = false;
	//}
	//else if (m_level->IsBox(actX, actY))
	//{
	//	if (m_input.BinaryChoice("SEARCH CHEST?")) {
	//		//random items?
	//		ChestLoot();
	//		///PlayPickupEffect();
	//		m_level->ClearSpace(actX, actY);
	//	}
	//	else {}
	//	system("cls");
	//	lvlDrawn = false;
	//}
	//else if (m_level->IsDoor(actX, actY))
	//{
	//	if(m_player->HasKey())
	//	{
	//		if (m_input.BinaryChoice("USE KEY ON DOOR?")) {
	//			m_level->ClearSpace(actX, actY);
	//			m_player->UseKey();
	//			///PlayDoorOpenEffect();
	//			cout << '\r' << " [DOOR OPENED]" << endl;
	//			system("pause");
	//		}
	//	}
	//	else
	//	{
	//		///PlayDoorClosedEffect();
	//		cout << '\r' << " [DOOR LOCKED]" << endl;
	//		system("pause");
	//	}
	//	system("cls");
	//	lvlDrawn = false;
	//}
	
}


void Game::ChestLoot() {
	//loot table: 1 key, 1 ore, 2 ore

	cout << '\r' << " [FOUND x";
	switch (int chestRoll = m_randomizer.Generate(1, 3))
	{
	case 1:
	{
		cout << "1";
		m_visuals.ColorText(AColor::Yellow);
		cout << " KEY";
		m_visuals.ResetTextColor();
		m_player->PickupKey(1);
		break;
	}
	case 2:
	{
		cout << "1";
		m_visuals.ColorText(AColor::Grey);
		cout << " ORE";
		m_visuals.ResetTextColor();
		m_player->PickupMat(1);
		break;
	}
	case 3:
	{
		cout << "2";
		m_visuals.ColorText(AColor::Grey);
		cout << " ORE";
		m_visuals.ResetTextColor();
		m_player->PickupMat(2);
		break;
	}
	default:
		break;
	}
	cout << "]" << endl;
	system("pause");
}