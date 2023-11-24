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
{

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
	m_visuals.DrawControls();
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

	//Movement inputs
	switch (input) {
	case '\t':
		m_player.OpenMenu();
		break;
	case 'w':
		newPlayerY--;
	case 'W':
	{
		m_player.SetLookDirection(1);
		break;
	}
	case 's':
		newPlayerY++;
	case 'S':
	{
		m_player.SetLookDirection(2);
		break;
	}
	case 'a':
		newPlayerX--;
	case 'A':
	{
		m_player.SetLookDirection(3);
		break;
	}
	case 'd':
		newPlayerX++;
	case 'D':
	{
		m_player.SetLookDirection(4);
		break;
	}
	default:
		break;
	}

	//Move player and check for unique space
	////int index = GetIndexFromCoordinates(newPlayerX, newPlayerY, width);
	if (m_level.IsSpace(newPlayerX, newPlayerY))
	{
		//Confine player to level edges
		if (newPlayerX < 0)
			newPlayerX = 0;
		else if (newPlayerX == m_level.GetWidth())
			newPlayerX = m_level.GetWidth() - 1;
		if (newPlayerY < 0)
			newPlayerY = 0;
		else if (newPlayerY == m_level.GetHeight())
			newPlayerY = m_level.GetHeight() - 1;
		m_player.SetPosition(newPlayerX, newPlayerY);
	}
	else if (m_level.IsGoal(newPlayerX, newPlayerY))
	{
		m_level.ClearSpace(newPlayerX, newPlayerY);
		m_player.SetPosition(newPlayerX, newPlayerY);
		m_level.ClearLevel();
		return true;
	}
	else {}
	return false;
}
void Game::Draw()
{
	system("cls");
	//m_visuals.SetLevelRef(m_level);
	m_visuals.DrawTop(m_level.GetWidth());

	for (int y = 0; y < m_level.GetHeight(); y++)
	{
		m_visuals.DrawLeft(y);

		for (int x = 0; x < m_level.GetWidth(); x++)
		{
			if (m_player.GetXPosition() == x && m_player.GetYPosition() == y) {
				m_player.Draw();
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
		m_visuals.DrawRight(y, m_level.GetWidth());

	}
	m_visuals.DrawBottom(m_level.GetWidth());
}

void Game::Interact(int x, int y)
{
	system("cls");
	Draw();
	int actX = x;
	int actY = y;
	switch (m_player.GetLookDirection())
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
	}
	else if (m_level.IsDoor(actX, actY))
	{
		if(m_player.HasKey())
		{
			if (m_input.BinaryChoice("USE KEY ON DOOR?")) {
				m_level.ClearSpace(actX, actY);
				m_player.UseKey();
				///PlayDoorOpenEffect();
				m_visuals.SubText("DOOR OPENED");
			}
		}
		else
		{
			///PlayDoorClosedEffect();
			m_visuals.SubText("DOOR LOCKED");
		}
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