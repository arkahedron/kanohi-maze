#include "Level.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <conio.h>
#include <windows.h>

using namespace std;

constexpr char WAL = (char)219;
constexpr char KEY = (char)126;
constexpr char DOR = (char)35;
constexpr char GOL = (char)234;
constexpr char MAT = (char)232; //15
constexpr char BOX = (char)239; //127

Level::Level()
	: m_pLevelData(nullptr)
	, m_height(0)
	, m_width(0)
	, m_holeX(0)
	, m_holeY(0)
	, m_levelsCleared(0)
	, m_levelToLoad(" ")
	, m_defaultLevel("Entry.txt")
{

}
Level::~Level()
{
	if (m_pLevelData != nullptr)
	{
		delete[] m_pLevelData;
		m_pLevelData = nullptr;
	}
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
void Level::Draw(int x, int y)
{
	int index = GetIndexFromCoordinates(x, y);
	cout << m_pLevelData[index];
}

bool Level::IsSpace(int x, int y)
{
	return m_pLevelData[GetIndexFromCoordinates(x, y)] == ' ';
}
bool Level::IsDoor(int x, int y)
{
	return m_pLevelData[GetIndexFromCoordinates(x, y)] == DOR;
}
bool Level::IsKey(int x, int y)
{
	return m_pLevelData[GetIndexFromCoordinates(x, y)] == KEY;
}
bool Level::IsMat(int x, int y)
{
	return m_pLevelData[GetIndexFromCoordinates(x, y)] == MAT;
}
bool Level::IsBox(int x, int y)
{
	return m_pLevelData[GetIndexFromCoordinates(x, y)] == BOX;
}
bool Level::IsGoal(int x, int y)
{
	return m_pLevelData[GetIndexFromCoordinates(x, y)] == GOL;
}

void Level::ClearSpace(int x, int y)
{
	m_pLevelData[GetIndexFromCoordinates(x, y)] = ' ';
}
void Level::ClearLevel() 
{
	m_levelsCleared++;
}

char Level::GetSpaceAtPosition(int x, int y) 
{
	return m_pLevelData[GetIndexFromCoordinates(x, y)];
}

bool Level::Convert(int* playerX, int* playerY)
{

	//Convert file line-by-line and check for unknown characters
	bool anyWarnings = false;
	for (int y = 0; y < m_height; y++)
	{
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
				m_pLevelData[index] = WAL;
				break;
			case 'D':
			case 'd':
				m_pLevelData[index] = DOR;
				break;
			case '*':
			case 'K':
			case 'k':
				m_pLevelData[index] = KEY;
				break;
			case 'M':
			case 'm':
				m_pLevelData[index] = MAT;
				break;
			case 'C':
			case 'c':
				m_pLevelData[index] = BOX;
				break;
			case 'X':
			case 'x':
				m_pLevelData[index] = GOL;
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
	}
	return anyWarnings;
}


int Level::GetIndexFromCoordinates(int x, int y)
{
	return x + y * m_width;
}