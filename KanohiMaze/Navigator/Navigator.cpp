#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <vector>

using namespace std;


//Unique symbol assignments
constexpr char kPlayerSymbol = 225;
constexpr char WAL = 219;
constexpr char KEY = 232;
constexpr char DOR = 177;
constexpr char GOL = 36;
//Border symbol assignments
constexpr char kTopRightBorder = 187;
constexpr char kTopLeftBorder = 201;
constexpr char kBottomRightBorder = 188;
constexpr char kBottomLeftBorder = 200;
constexpr char kHorizontalBorder = 205;
constexpr char kVerticalBorder = 186;
//Text color assignments
constexpr int kOpenDoorColor = 10;
constexpr int kClosedDoorColor = 12;
constexpr int kRegularColor = 7;

//Level construction
string SelectLevelFromList();
char* LoadLevel(string levelName, int& width, int& height);
bool ConvertLevel(char* level, int width, int height, int& playerX, int& playerY);

//Map gameplay managers
int GetIndexFromCoordinates(int x, int y, int width);
void DrawLevel(char level[], int width, int height, int playerX, int playerY, bool playerHasKey);
bool UpdatePlayerPosition(char level[], int& playerX, int& playerY, int width, bool& playerHasKey);

//State effect managers
void PlayDoorClosedEffect();
void PlayDoorOpenEffect();
void PlayKeyPickupEffect();
void PlayWinEffect();

//Border constructors
void DisplayTopBorder(int width);
void DisplayBottomBorder(int width);
void DisplayLeftBorder();
void DisplayRightBorder();

//Action managers
bool DoBinaryAction();
void PrintMainText(string mainText);
void PrintSubText(string subText);

//Global variable initializers
char mainInput = ' ';
string mainText = " ";
string subText = " ";
bool inMapMode = false;

//Size and assign level map
int width = 0;
int height = 0;
char* levelArray;

//Initialize map variables
int playerX = 1;
int playerY = 1;
bool playerHasKey = false;
bool gameOver = false;
bool isMapMode = true;
bool isInventoryMode = false;
bool continuePlaying = true;

//Main code body
int main()
{
	cout << "---{ WELCOME }---" << endl;

	do {
		//Select, import, and convert level, then check for errors
		string levelName = SelectLevelFromList();
		levelArray = LoadLevel(levelName, width, height);
		bool anyWarnings = ConvertLevel(levelArray, width, height, playerX, playerY);
		if (anyWarnings)
		{
			cout << "There were some warnings in the level data, see above." << endl;
			system("pause");
		}

		PrintMainText("YOU ENTER THE MAZE");

		//Main game Loop
		while (!gameOver)
		{
			if (isMapMode)
			{
				system("cls");
				DrawLevel(levelArray, width, height, playerX, playerY, playerHasKey);
				gameOver = UpdatePlayerPosition(levelArray, playerX, playerY, width, playerHasKey);
			}
			else if (isInventoryMode)
			{
				system("cls");

			}
			else {}
		}

		system("cls");
		DrawLevel(levelArray, width, height, playerX, playerY, playerHasKey);

		//End Game
		PlayWinEffect();
		PrintMainText("YOU WIN");
		delete[] levelArray;

		mainText = "PLAY AGAIN?";
		continuePlaying = DoBinaryAction();
		gameOver = false;
	} while (continuePlaying);
}


//Level initializers
string SelectLevelFromList() 
{
	cout << "What level would you like to load?" << endl;

	string selectedLevel;
	int levelNum = 0;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(L"../Levels/*", &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					wstring wsLevel(FindFileData.cFileName);
					string levelStr(wsLevel.begin(), wsLevel.end());
					if (bool isTextFile = (levelStr.substr(levelStr.find_last_of(".") + 1) == "txt"))
					{
						levelNum++;
						cout << " > " << levelNum << ": " << levelStr << "\n";
					}
				}
			} while (FindNextFile(hFind, &FindFileData) != 0);
		FindClose(hFind);

		string levelInput;
		cin >> levelInput;
		return levelInput;
	}
	else {
		cout << "No valid levels found!" << endl;
	}
}
char* LoadLevel(string levelName, int& width, int& height)
{
	levelName.insert(0, "../Levels/");
	ifstream levelFile;
	levelFile.open(levelName);
	if(levelFile)
	{
		constexpr int tempSize = 25;
		char temp[tempSize];

		levelFile.getline(temp, tempSize, '\n');
		width = atoi(temp);

		levelFile.getline(temp, tempSize, '\n');
		height = atoi(temp);

		char* levelData = new char[width * height];
		levelFile.read(levelData, width * height);
		return levelData;

	}
	else
	{
		cout << "Opening file failed!" << endl;
		return LoadLevel(SelectLevelFromList(),width, height);
	}
}
bool ConvertLevel(char* level, int width, int height, int& playerX, int& playerY)
{
	bool anyWarnings = false;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int index = GetIndexFromCoordinates(x, y, width);

			switch (level[index])
			{
				case '+':
				case '-':
				case '|':
				case 'o':
					level[index] = WAL;
					break;
				case '*':
				case 'k':
					level[index] = KEY;
					break;
				case 'D':
				case 'd':
					level[index] = DOR;
					break;
				case 'X':
				case 'x':
					level[index] = GOL;
					break;
				case '@':
				case 'p':
					level[index] = ' ';
					playerX = x;
					playerY = y;
					break;
				case ' ':
					break;
				default:
					cout << "Invalid character in level file: " << level[index] << endl;
					anyWarnings = true;
					break;
			}
		}
	}

	return anyWarnings;
}

//Map gameplay managers
int GetIndexFromCoordinates(int x, int y, int width)
{
	return x + y * width;
}
void DrawLevel(char level[], int width, int height, int playerX, int playerY, bool playerHasKey)
{
	cout << "\n\n";
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	DisplayTopBorder(width);

	for (int y = 0; y < height; y++)
	{
		DisplayLeftBorder();

		for (int x = 0; x < width; x++)
		{
			if (playerX == x && playerY == y)
			{
				SetConsoleTextAttribute(console, 11);
				cout << kPlayerSymbol;
				SetConsoleTextAttribute(console, kRegularColor);
			}
			else
			{
				int indexToPrint = GetIndexFromCoordinates(x, y, width);
		
				if (level[indexToPrint] == DOR)
				{
					if (playerHasKey)
					{
						SetConsoleTextAttribute(console, kOpenDoorColor);
					}
					else
					{
						SetConsoleTextAttribute(console, kClosedDoorColor);
					}
				}
				else if (level[indexToPrint] == GOL)
				{
					SetConsoleTextAttribute(console, 14);
				}
				else 
				{
					SetConsoleTextAttribute(console, kRegularColor);
				}

				cout << level[indexToPrint];
			}

		}
		DisplayRightBorder();
	}
	DisplayBottomBorder(width);

}
bool UpdatePlayerPosition(char level[], int& playerX, int& playerY, int width, bool& playerHasKey)
{
	char input = _getch();

	int newPlayerX = playerX;
	int newPlayerY = playerY;

	//Movement inputs
	switch (input)
	{
		case 'w':
		case 'W':
		{
			newPlayerY--;
			break;
		}
		case 's':
		case 'S':
		{
			newPlayerY++;
			break;
		}
		case 'a':
		case 'A':
		{
			newPlayerX--;
			break;
		}
		case 'd':
		case 'D':
		{
			newPlayerX++;
			break;
		}
		case '\t':
		{
			isMapMode = false;
			isInventoryMode = true;
			break;
		}
		default:
			break;
	}

	//Check for unique space 
	int index = GetIndexFromCoordinates(newPlayerX, newPlayerY, width);
	if (level[index] == ' ')
	{
		if (newPlayerX < 0)
			newPlayerX = 0;
		else if (newPlayerX == width)
			newPlayerX = width - 1;
		if (newPlayerY < 0)
			newPlayerY = 0;
		else if (newPlayerY == height)
			newPlayerY = height - 1;

		playerX = newPlayerX;
		playerY = newPlayerY;
	}
	else if (level[index] == KEY)
	{
		mainText = "COLLECT KEY?";
		if (DoBinaryAction()) {
			playerHasKey = true;
			level[index] = ' ';
			playerX = newPlayerX;
			playerY = newPlayerY;
			PlayKeyPickupEffect();
			PrintSubText("KEY COLLECTED");
		} else { }
	}
	else if (level[index] == DOR && playerHasKey)
	{
		mainText = "UNLOCK DOOR?";
		if (DoBinaryAction()) {
			level[index] = ' ';
			playerHasKey = false;
			playerX = newPlayerX;
			playerY = newPlayerY;
			PlayDoorOpenEffect();
			PrintSubText("DOOR OPENED");
		} else { }
	}
	else if (level[index] == DOR && !playerHasKey)
	{
		PlayDoorClosedEffect();
		PrintSubText("DOOR LOCKED");
	}
	else if (level[index] == GOL)
	{
		level[index] = ' ';
		playerX = newPlayerX;
		playerY = newPlayerY;
		return true;
	}
	return false;
}

//Action managers
bool DoBinaryAction()
{
	system("cls");
	cout << endl << "  [ " << mainText << " ]" << endl << "> y/n" << endl;
	mainInput = _getch();

	switch (mainInput)
	{
	case 'y':
	case 'Y':
		isMapMode = true;
		return true;
		break;
	case 'n':
	case 'N':
		isMapMode = true;
		return false;
		break;
	default:
		return DoBinaryAction();
		break;
	}
}
void PrintMainText(string mainText) {
	system("cls");
	cout << endl << "  [" << mainText << "]" << endl;
	system("pause");
}
void PrintSubText(string subText)
{
	system("cls");
	DrawLevel(levelArray, width, height, playerX, playerY, playerHasKey);
	cout << endl << "  [" << subText << "]" << endl;
	system("pause");
}

//Play state effects
void PlayDoorClosedEffect()
{
	

	Beep(500, 75);
	Beep(500, 75);
}
void PlayDoorOpenEffect()
{
	Beep(1397, 200);
}
void PlayKeyPickupEffect()
{	
	
	Beep(1568, 200);
}
void PlayWinEffect()
{
	Beep(1245, 150);
	Beep(1397, 150);
	Beep(1568, 200);
	Beep(1868, 400);
}

//Print map borders
void DisplayTopBorder(int width)
{
	cout << "  " << kTopLeftBorder;
	for (int i = 0; i < width; i++)
	{
		cout << kHorizontalBorder;
	}
	cout << kTopRightBorder << endl;
}
void DisplayBottomBorder(int width)
{
	cout << "  " << kBottomLeftBorder;
	for (int i = 0; i < width; i++)
	{
		cout << kHorizontalBorder;
	}
	cout  << kBottomRightBorder << endl;
}
void DisplayLeftBorder()
{
	cout << "  " << kVerticalBorder;
}
void DisplayRightBorder()
{
	cout << kVerticalBorder << endl;
}
