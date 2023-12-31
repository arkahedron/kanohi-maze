#include <iostream>
#include <fstream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <string>

#include "Level.h"
#include "Player.h"
#include "Enemy.h"
#include "Key.h"
#include "Door.h"
#include "Wall.h"
#include "Goal.h"
#include "Chest.h"
#include "Ore.h"

using namespace std;

constexpr char WAL = (char)219;
constexpr char GOL = (char)234;

static bool edgesDrawn;

Level::Level()
	: m_pLevelData(nullptr)
	, m_height(0)
	, m_width(0)
	, m_holeX(0)
	, m_holeY(0)
	, m_levelsCleared(0)
	, m_levelToLoad(" ")
	, m_defaultLevel("Entry.txt")
	, m_levelDrawn(false)
	, edgeHit(false)
	, edgeShiftBuffer()
	, minCorner()
	, maxCorner()

{
	m_visuals.SetLevelRef(this);
}
Level::~Level()
{
	if (m_pLevelData != nullptr)
	{
		delete[] m_pLevelData;
		m_pLevelData = nullptr;

	}
	while (!m_pActors.empty())
	{
		delete m_pActors.back();
		m_pActors.pop_back();
	}
}
Level* Level::instance = nullptr;
Level* Level::GetInstance()
{
	if (instance == nullptr) {
		instance = new Level();
	}
	return instance;
}

string Level::SelectNewLevel()
{
	system("cls");
	if (m_levelsCleared > 0)
	{
		LPCWSTR lpath = L"../Levels/*.txt";
		vector<wstring> fileListArray;
		wstring lvlSelect;
		int lvlIndex = -1;

		WIN32_FIND_DATAW ffd;
		HANDLE hFind = FindFirstFileW(lpath, &ffd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do {
				if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
				{
					lvlIndex++;
					fileListArray.push_back(ffd.cFileName);
				}
			} while (FindNextFileW(hFind, &ffd));
			FindClose(hFind);
		}
		lvlSelect = fileListArray[m_randomizer.Generate(0, lvlIndex)];
		string selectedLevel(lvlSelect.begin(), lvlSelect.end());
		//cout << endl << selectedLevel << endl;

		//while (!fileListArray.empty())
		//{
		//	delete &fileListArray.back();
		//	fileListArray.pop_back();
		//}

		if (selectedLevel == m_levelToLoad || selectedLevel == m_defaultLevel) {
			return SelectNewLevel();
		}
		else {
			return selectedLevel;
		}
	}
	else 
	{ 
		/*cout << "YOU ENTER THE MAZE" << endl;
		system("pause"); */
		return m_defaultLevel;
	}
	
}

bool Level::Load(string levelName, int* playerX, int* playerY)
{
	m_levelToLoad = levelName;
	//Get defined text file as level and try to open it
	levelName.insert(0, "../Levels/");
	ifstream activeLevelFile;

	activeLevelFile.open(levelName);
	if (!activeLevelFile)
	{
		cout << " Invalid file, level failed to load!" << endl;
		//return LoadLevel(SelectLevelFromList(true), width, height);
		return false;
	}
	else {
		constexpr int tempSize = 25;
		char temp[tempSize];

		activeLevelFile.getline(temp, tempSize, '\n');
		m_width = atoi(temp);

		activeLevelFile.getline(temp, tempSize, '\n');
		m_height = atoi(temp);

		m_pLevelData = new char[m_width * m_height];
		activeLevelFile.read(m_pLevelData, (long long)m_width * (long long)m_height);

		//levelArray = LoadLevel(levelName, width, height);
		bool anyWarnings = Convert(playerX, playerY);
		if (anyWarnings)
		{
			cout << "There were some warnings in the level data, see above." << endl;
			system("pause");
		}
		//return levelData;
		return true;
	}

}

bool Level::Convert(int* playerX, int* playerY)
{
	bool anyWarnings = false;

	//Top Edge Walls
	for (int t = -1; t < m_width+1; t++)
	{ m_pActors.push_back(new Wall(t, -1)); }

	for (int y = 0; y < m_height; y++)
	{
		//Left Edge Walls
		m_pActors.push_back(new Wall(-1, y));

		for (int x = 0; x < m_width; x++)
		{

			int index = GetIndexFromCoordinates(x, y);

			//Level conversion key
			switch (m_pLevelData[index]) {
			case '+':
			case '-':
			case '|':
			case 'O':
			case 'o':
				m_pLevelData[index] = ' ';
				m_pActors.push_back(new Wall(x, y));
				break;
			case 'D':
			case 'd':
				m_pLevelData[index] = ' ';
				m_pActors.push_back(new Door(x, y));
				break;
			case '*':
			case 'K':
			case 'k':
				m_pLevelData[index] = ' ';
				m_pActors.push_back(new Key(x, y));
				break;
			case 'M':
			case 'm':
				m_pLevelData[index] = ' ';
				m_pActors.push_back(new Ore(x, y));
				break;
			case 'C':
			case 'c':
				m_pLevelData[index] = ' ';
				m_pActors.push_back(new Chest(x, y));
				break;
			case 'X':
			case 'x':
				m_pLevelData[index] = ' ';
				m_pActors.push_back(new Goal(x, y));
				m_holeY = y;
				m_holeX = x;
				break;
			case '@':
			case 'P':
			case 'p':
				m_pLevelData[index] = ' ';

				if (playerX != nullptr && playerY != nullptr)
				{
					*playerX = x;
					*playerY = y;
				}
				break;
			case ' ':
				break;
			default:
				cout << "Invalid character in level file: " << m_pLevelData[index] << endl;
				anyWarnings = true;
				break;
			}
		}
		//Right Edge Walls
		m_pActors.push_back(new Wall(m_width, y));
	}
	//Bottom Edge Walls
	for (int t = -1; t < m_width+1; t++)
	{ m_pActors.push_back(new Wall(t, m_height)); }
	
	AlignScreenToPlayer(playerX, playerY);
	
	edgesDrawn = false;
	return anyWarnings;
}

void Level::AlignScreenToPlayer(int* playerX, int* playerY)
{
	COORD screenCenter = {};
	screenCenter.X = round(m_visuals.edgeWidth / 2);
	screenCenter.Y = round(m_visuals.edgeHeight / 2);

	//edgeShiftBuffer = { (0),(0) };
	edgeShiftBuffer.X = (screenCenter.X - *playerX) / 2;
	edgeShiftBuffer.Y = (screenCenter.Y - *playerY) / 2;

	minCorner = { (0),(0) };
	maxCorner.X = m_visuals.edgeWidth;
	maxCorner.Y = m_visuals.edgeHeight;
}

char Level::GetSpaceAtPosition(int x, int y)
{
	return m_pLevelData[GetIndexFromCoordinates(x, y)];
}

void Level::ClearSpace(int x, int y, WorldActor* delActor)
{
	if (delActor != nullptr)
	{
		for (auto actor = m_pActors.begin(); actor != m_pActors.end(); ++actor)
		{
			WorldActor* refActor = (*actor);
			if (refActor == delActor)
			{
				m_pActors.erase(actor);
				break;
			}
		}
	}
	m_visuals.DrawAtSpace(x, y, ' ');
}
void Level::ClearLevel() 
{
	if (m_pLevelData != nullptr)
	{
		delete[] m_pLevelData;
		m_pLevelData = nullptr;

	}
	while (!m_pActors.empty())
	{
		delete m_pActors.back();
		m_pActors.pop_back();
	}
	SetDrawnState(false);
	m_levelsCleared++;
}

void Level::Draw()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	edgeHit = HandlePlayerEdgeHit();

	if (edgeHit) { CleanUpScreen(); }

	//Updating actors in level and drawing those within screen bounds
	for (auto actor = m_pActors.begin(); actor != m_pActors.end(); ++actor)
	{
		if ((*actor)->IsActive())
		{
			if ((*actor)->GetType() == ActorType::Door) { (*actor)->Update(); }

			COORD actorRelativeDrawPos = {};
			actorRelativeDrawPos.X = (*actor)->GetXPosition() + edgeShiftBuffer.X;
			actorRelativeDrawPos.Y = (*actor)->GetYPosition() + edgeShiftBuffer.Y;
			(*actor)->ModDrawPos(edgeShiftBuffer);

			bool isWithinScreenBounds = false;
			if(actorRelativeDrawPos.X > minCorner.X-1 && actorRelativeDrawPos.Y > minCorner.Y-1)
			{ 
				if (actorRelativeDrawPos.X < maxCorner.X-2 && actorRelativeDrawPos.Y < maxCorner.Y+1)
				{ isWithinScreenBounds = true; }
			}

			if (isWithinScreenBounds) { (*actor)->Draw(); }

		}
	}

	//Correct and draw player on screen
	Player::GetInstance()->m_WorldActor.ModDrawPos(edgeShiftBuffer);
	Player::GetInstance()->m_WorldActor.Draw();

	//Draw frame and HUD
	if(!edgesDrawn){
		m_visuals.DrawFrame();
		m_visuals.DrawMazeControls();
		m_visuals.ResetCursor({ (0),(0) }, true);
		edgesDrawn = true;
	}
	
}

WorldActor* Level::GetActorAtPos(int x, int y)
{
	WorldActor* targetActor = nullptr;
	for (auto actor = m_pActors.begin(); actor != m_pActors.end(); ++actor)
	{
		if ((*actor)->IsActive())
		{
			if (x == (*actor)->GetXPosition() && y == (*actor)->GetYPosition())
			{
				targetActor = (*actor);
			}
		}
	}
	return targetActor;
}

int Level::GetIndexFromCoordinates(int x, int y)
{
	return x + y * m_width;
}

void Level::SetDrawnState(bool isDrawn) 
{
	if(!isDrawn)
	for (int y = minCorner.Y; y < maxCorner.Y + 1; ++y)
	{
		for (int x = minCorner.X; x < maxCorner.X - 2; ++x)
		{
			m_visuals.DrawAtSpace(x, y, ' ');
		}
	}
	m_levelDrawn = isDrawn; 
	edgesDrawn = isDrawn;
};

void Level::CleanUpScreen()
{
	for (int y = minCorner.Y; y < maxCorner.Y + 1; ++y)
	{
		for (int x = minCorner.X; x < maxCorner.X - 2; ++x)
		{
			m_visuals.DrawAtSpace(x, y, ' ');
		}
	}
}

bool Level::HandlePlayerEdgeHit()
{
	int playX = Player::GetInstance()->m_WorldActor.m_drawPos.X;
	int playY = Player::GetInstance()->m_WorldActor.m_drawPos.Y;

	if (playX <= 0)
	{
		edgeShiftBuffer.X++;
		return true;
	}
	else if (playY <= 0)
	{
		edgeShiftBuffer.Y++;
		return true;
	}
	else if (playX >= m_visuals.edgeWidth - 4)
	{
		edgeShiftBuffer.X--;
		return true;
	}
	else if (playY >= m_visuals.edgeHeight)
	{
		edgeShiftBuffer.Y--;
		return true;
	}
	else 
	{ 
		return false; 
	}

}