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

#include <assert.h>

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
	for (int t = -1; t < m_width+1; t++)
	{ m_pActors.push_back(new Wall(t, -1)); }

	for (int y = 0; y < m_height; y++)
	{
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
		m_pActors.push_back(new Wall(m_width, y));
	}
	for (int t = -1; t < m_width+1; t++)
	{ m_pActors.push_back(new Wall(t, m_height)); }
	

	COORD screenCenter = {};
	screenCenter.X = round(m_visuals.edgeWidth/2);
	screenCenter.Y = round(m_visuals.edgeHeight/2);
	

	

	//edgeShiftBuffer = { (0),(0) };
	edgeShiftBuffer.X = (screenCenter.X - *playerX) / 2; 
	edgeShiftBuffer.Y = (screenCenter.Y - *playerY) / 2;
	
	minCorner = { (0),(0) };
	maxCorner.X = m_visuals.edgeWidth;
	maxCorner.Y = m_visuals.edgeHeight;
	edgesDrawn = false;
	return anyWarnings;
}

char Level::GetSpaceAtPosition(int x, int y)
{
	return m_pLevelData[GetIndexFromCoordinates(x, y)];
}
bool Level::IsSpace(int x, int y)
{
	return m_pLevelData[GetIndexFromCoordinates(x, y)] == ' ';
}
bool Level::IsGoal(int x, int y)
{
	return m_pLevelData[GetIndexFromCoordinates(x, y)] == GOL;
}

void Level::ClearSpace(int x, int y, WorldActor* actorToDelete)
{
	m_pLevelData[GetIndexFromCoordinates(x, y)] = ' ';
	
	if (actorToDelete != nullptr)
	{
		for (auto actor = m_pActors.begin(); actor != m_pActors.end(); ++actor)
		{
			WorldActor* refActor = (*actor);
			if (refActor == actorToDelete)
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
	//system("cls");
	////////////START OF ENCAP EXPERIMENT
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD finalScreenPos = {};

	int playX = Player::GetInstance()->m_WorldActor.m_drawPos.X;
	int playY = Player::GetInstance()->m_WorldActor.m_drawPos.Y;

	COORD oldPlayPos = Player::GetInstance()->oldPlayerCoord;

	bool edgeHit;

	if (playX <= 0)
	{
		//maxCorner.X += 1; 
		//minCorner.X += 1; 
		edgeShiftBuffer.X++;
		edgeHit = true;
	}
	if (playY <= 0)
	{
		edgeShiftBuffer.Y++;
		edgeHit = true;
	}
	if (playX >= m_visuals.edgeWidth-4)
	{
		edgeShiftBuffer.X--;
		edgeHit = true;
	}
	if (playY >= m_visuals.edgeHeight)
	{
		edgeShiftBuffer.Y--;
		edgeHit = true;
	}



	COORD oldActorPos = { (0),(0) };
	for (auto actor = m_pActors.begin(); actor != m_pActors.end(); ++actor)
	{
		if ((*actor)->IsActive())
		{
			oldActorPos.X = (*actor)->GetXPosition();
			oldActorPos.Y = (*actor)->GetYPosition();
	
			oldActorPos.X += edgeShiftBuffer.X;
			oldActorPos.Y += edgeShiftBuffer.Y;
	
			(*actor)->ModDrawPos(edgeShiftBuffer);
			bool isWithinScreenBounds = false;
			if(oldActorPos.X > minCorner.X-1 && oldActorPos.Y > minCorner.Y-1)
			{ 
				if (oldActorPos.X < maxCorner.X-2 && oldActorPos.Y < maxCorner.Y+1)
				{ isWithinScreenBounds = true; }
			}

			if (isWithinScreenBounds)
			{
				(*actor)->Draw();
				//if ((*actor)->GetType() == ActorType::Door) { (*actor)->Update(); }
			}
	
		}
	}
	Player::GetInstance()->m_WorldActor.ModDrawPos(edgeShiftBuffer);
	Player::GetInstance()->m_WorldActor.Draw();

	do {
		m_visuals.DrawFrame();
		m_visuals.DrawMazeControls();
		m_visuals.ResetCursor({ (0),(0) }, true);
		edgeHit = false;
	} while (edgeHit == true);

	////////////END OF ENCAP EXPERIMENT


	//if (m_pLevelData)
	//{
	//	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	//	m_visuals.ResetTextColor();
	//	m_visuals.ResetCursor(); /*required for correct map placement*/
	//	if (!m_levelDrawn)
	//	{
	//		m_visuals.DrawTop();
	//		for (int y = 0; y < m_height; ++y)
	//		{
	//			m_visuals.DrawLeft(y);
	//			for (int x = 0; x < m_width; ++x)
	//			{
	//				int indexToPrint = GetIndexFromCoordinates(x, y);
	//				cout << m_pLevelData[indexToPrint];
	//			}
	//			m_visuals.DrawRight(y);
	//		}
	//		m_visuals.DrawBottom();
	//		m_visuals.DrawMazeControls();


	//		COORD actorCursorPosition = {(0),(0)};
	//	for (auto actor = m_pActors.begin(); actor != m_pActors.end(); ++actor)
	//	{
	//		if ((*actor)->IsActive())
	//		{
	//			actorCursorPosition.X = (*actor)->GetXPosition();
	//			actorCursorPosition.Y = (*actor)->GetYPosition();
	//			SetConsoleCursorPosition(console, actorCursorPosition);
	//			(*actor)->Draw();
	//			if ((*actor)->GetType() == ActorType::Door)
	//			{
	//				(*actor)->Update();
	//			}
	//		
	//		}
	//	}
	//	m_levelDrawn = true;
	//	}
	//	Player::GetInstance()->m_WorldActor.Draw();
	//}
}

WorldActor* Level::UpdateActors(int x, int y)
{
	WorldActor* collidedActor = nullptr;

	for (auto actor = m_pActors.begin(); actor != m_pActors.end(); ++actor)
	{
		if ((*actor)->IsActive())
		{
			//updates all actors
			(*actor)->Update(); 
			if ((*actor)->GetXPositionPointer() != nullptr && (*actor)->GetYPositionPointer() != nullptr)
			{
				if (x == (*actor)->GetXPosition() && y == (*actor)->GetYPosition())
				{
					//only collide with one actor
					assert(collidedActor == nullptr);
					collidedActor = (*actor);
				}
			}
		}
		else { delete (*actor); }
	}
	return collidedActor;
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
	m_levelDrawn = isDrawn; 
	edgesDrawn = isDrawn;
};