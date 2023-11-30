#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;


constexpr int colorBase = 7;
constexpr int colorKey = 14;
constexpr int colorMat = 8;
constexpr int colorBox = 6;


Game::Game()
	: levelEnd(false)
	, exitedGame(false)
	, m_levelName("Entry.txt")
	, m_visuals()
	, lvlDrawn(false)
	, roomsCleared(0)
{
	m_visuals.SetLevelRef(&m_level);
}
Game::~Game()
{

}

bool Game::Load()
{
	m_levelName = m_level.SelectNewLevel();
	return m_level.Load(m_levelName, m_player.GetXPositionPointer(), m_player.GetYPositionPointer());
}
void Game::Run()
{
	Draw();
	levelEnd = Update();
	exitedGame = m_player.exited;

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
	int newPlayerX = m_player.GetXPosition();
	int newPlayerY = m_player.GetYPosition();

	
	if(input == 'e' || input == 'E')
	{ Interact(newPlayerX, newPlayerY);}
	m_visuals.DrawAtSpace(m_player.GetXPosition(), m_player.GetYPosition(), m_level.GetSpaceAtPosition(m_player.GetXPosition(), m_player.GetYPosition()));

	//Movement inputs
	switch (input) 
	{
	case '\t':
		m_player.OpenMenu();
		lvlDrawn = false;
		break;
	case 'w': newPlayerY--;
	case 'W':
	{ m_player.SetLookDirection(1); break; }
	case 's': newPlayerY++;
	case 'S':
	{ m_player.SetLookDirection(2); break; }
	case 'a': newPlayerX--;
	case 'A':
	{ m_player.SetLookDirection(3); break; }
	case 'd': newPlayerX++;
	case 'D':
	{ m_player.SetLookDirection(4); break; }
	default: break;
	}

	//Move player and check for unique space
	if (m_level.IsSpace(newPlayerX, newPlayerY))
	{
		//Confine player to level edges
		if (newPlayerX < 0)
			newPlayerX = 0;
		else if (newPlayerX == m_level.m_width)
			newPlayerX = m_level.m_width - 1;
		if (newPlayerY < 0)
			newPlayerY = 0;
		else if (newPlayerY == m_level.m_height)
			newPlayerY = m_level.m_height - 1;
		m_player.SetPosition(newPlayerX, newPlayerY);
	}
	else if (m_level.IsGoal(newPlayerX, newPlayerY))
	{
		m_level.ClearSpace(newPlayerX, newPlayerY);
		m_player.SetPosition(newPlayerX, newPlayerY);
		m_level.ClearLevel();
		roomsCleared++;
		lvlDrawn = false;
		return true;
	}
	else {}
	m_visuals.DrawAtSpace(m_player.GetXPosition(), m_player.GetYPosition(), m_player.Draw());
	m_visuals.ColorText(colorBase);
	return false;
}

void Game::Draw()
{
	if (!lvlDrawn)
	{
		m_visuals.ResetCursor(); /*required for correct map placement*/
		m_visuals.DrawTop();
		for (int y = 0; y < m_level.m_height; y++)
		{
			m_visuals.DrawLeft(y);
			for (int x = 0; x < m_level.m_width; x++)
			{
				//Actual Per-Space print
				if (m_player.GetXPosition() == x && m_player.GetYPosition() == y) {
					cout << m_player.Draw();
				}
				else
				{
					//Colorize other unique symbols
					if (m_level.IsDoor(x, y)) {
						if (m_player.HasKey()) { m_visuals.ColorText(2); }
						else { m_visuals.ColorText(4); }
					}
					else if (m_level.IsKey(x, y)) { m_visuals.ColorText(colorKey); }
					else if (m_level.IsMat(x, y)) { m_visuals.ColorText(colorMat); }
					else if (m_level.IsBox(x, y)) { m_visuals.ColorText(colorBox); }
					else if (m_level.IsGoal(x, y)) { m_visuals.ColorText(15); }
					else { m_visuals.ColorText(colorBase); }
					//Print relevant symbol and reset color to base
					m_level.Draw(x, y);
					m_visuals.ColorText(colorBase);
				}
			}
			m_visuals.DrawRight(y);
		}
		m_visuals.DrawBottom();
		m_visuals.DrawControls();
	}
	lvlDrawn = true;
}

void Game::Interact(int x, int y)
{
	int actX = x;
	int actY = y;
	switch (m_player.lookDirection)
	{
	case 1: /*facing up*/
	{ actY--; break; }
	case 2: /*facing down*/
	{ actY++; break; }
	case 3: /*facing left*/
	{ actX--; break; }
	case 4: /*facing right*/
	{ actX++; break; }
	default: break;
	}
	//Interact with space player is facing
	char interactedWith = m_level.GetSpaceAtPosition(actX, actY);

	if (m_level.IsKey(actX, actY))
	{
		if (m_input.BinaryChoice("COLLECT KEY?")) {
			m_player.PickupKey(1);
			m_level.ClearSpace(actX, actY);
			///PlayPickupEffect();
			//m_visuals.SubText("KEY COLLECTED");
		}
		else {}
		system("cls");
		lvlDrawn = false;
	}
	else if (m_level.IsMat(actX, actY))
	{
		if (m_input.BinaryChoice("COLLECT ORE?")) {
			m_player.PickupMat(1);
			m_level.ClearSpace(actX, actY);
			///PlayPickupEffect();
			//m_visuals.SubText("ORE COLLECTED");
		}
		else {}
		system("cls");
		lvlDrawn = false;
	}
	else if (m_level.IsBox(actX, actY))
	{
		if (m_input.BinaryChoice("SEARCH CHEST?")) {
			//random items?
			ChestLoot();
			///PlayPickupEffect();
			m_level.ClearSpace(actX, actY);
		}
		else {}
		system("cls");
		lvlDrawn = false;
	}
	else if (m_level.IsDoor(actX, actY))
	{
		if(m_player.HasKey())
		{
			if (m_input.BinaryChoice("USE KEY ON DOOR?")) {
				m_level.ClearSpace(actX, actY);
				m_player.UseKey();
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
	}
	
}


void Game::ChestLoot() {
	//loot table: 1 key, 1 ore, 2 ore

	cout << '\r' << " [FOUND x";
	switch (int chestRoll = m_randomizer.Generate(1, 3))
	{
	case 1:
	{
		cout << "1";
		m_visuals.ColorText(14);
		cout << " KEY";
		m_visuals.ColorText(7);
		m_player.PickupKey(1);
		break;
	}
	case 2:
	{
		cout << "1";
		m_visuals.ColorText(8);
		cout << " ORE";
		m_visuals.ColorText(7);
		m_player.PickupMat(1);
		break;
	}
	case 3:
	{
		cout << "2";
		m_visuals.ColorText(8);
		cout << " ORE";
		m_visuals.ColorText(7);
		m_player.PickupMat(2);
		break;
	}
	default:
		break;
	}
	cout << "]" << endl;
	system("pause");
}