#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <vector>

using namespace std;

string defaultLevel = "Closet.txt";
bool manualLevelSelect;
//Custom text flavoring
char kPlayerSymbol = 4;
constexpr char WAL = 219;
constexpr char KEY = 126;
constexpr char DOR = 177;
constexpr char GOL = 234;
constexpr int kBaseColor = 7;


//Level construction
string SelectLevelFromList(bool manualSelect);
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
bool DoBinaryAction(string prompt, bool wipe);
void PrintMainText(string mainText);
void PrintSubText(string subText);

void OpenMenu();


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
bool isMenuMode = false;
bool continuePlaying = true;
int lookDirection = 0;

//Main code body
int main()
{
	cout << " ---{ WELCOME TO KANOHI MAZE }---" << endl;

	do {
		manualLevelSelect = DoBinaryAction(" Select Level Manually?", false);

		//Select, import, and convert level, then check for errors
		string levelName = SelectLevelFromList(manualLevelSelect);
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
				cout << " > Move: WASD" << endl;
				cout << " > Look: SHIFT+WASD" << endl;
				cout << " > Act: E" << endl;
				//? cout << " > Attack: Q" << endl;
				cout << " > Menu: TAB" << endl;

				gameOver = UpdatePlayerPosition(levelArray, playerX, playerY, width, playerHasKey);
			}
			else if (isMenuMode)
			{
				OpenMenu();
			}
			else {}

			if (!continuePlaying) { break; }
		}
		if (!continuePlaying) { break; }

		system("cls");
		DrawLevel(levelArray, width, height, playerX, playerY, playerHasKey);


		//End game and prompt new level
		PlayWinEffect();
		PrintMainText("GAME OVER");
		delete[] levelArray;
		continuePlaying = DoBinaryAction("PLAY AGAIN?",false);
		gameOver = false;
	} while (continuePlaying);
	cout << " exiting game..." << endl;
}


//Level initializers
string SelectLevelFromList(bool manualSelect) 
{
	string selectedLevel;
	int levelNum = 0;
	
	if (manualSelect)
	{
		cout << " What level would you like to load?" << endl;

		//Search for and print out potential valid text files as levels
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

			//Player input to select level, will be overhauled once map tethering is implemented
			string levelInput;
			cin >> levelInput;
			return levelInput;
		}
		else {
			cout << " No valid levels found!" << endl;
		}
	}
	else {
		return defaultLevel;
	}
}
char* LoadLevel(string levelName, int& width, int& height)
{
	//Get defined text file as level and try to open it
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
		return LoadLevel(SelectLevelFromList(true),width, height);
	}
}
bool ConvertLevel(char* level, int width, int height, int& playerX, int& playerY)
{
	//Convert file line-by-line and check for unknown characters
	bool anyWarnings = false;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int index = GetIndexFromCoordinates(x, y, width);

			//Conversion codes
			switch (level[index]) {
				case '+':
				case '-':
				case '|':
				case 'o':
				case 'O':
					level[index] = WAL;
					break;
				case '*':
				case 'k':
				case 'K':
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
				case 'P':
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
			if (playerX == x && playerY == y) {
				SetConsoleTextAttribute(console, 11);
				cout << kPlayerSymbol;
				SetConsoleTextAttribute(console, kBaseColor);
			}
			else
			{
				int indexToPrint = GetIndexFromCoordinates(x, y, width);
		
				if (level[indexToPrint] == DOR)
				{
					if (playerHasKey) { 
						SetConsoleTextAttribute(console, 2); 
					}
					else{ 
						SetConsoleTextAttribute(console, 4); 
					}
				}
				else if (level[indexToPrint] == GOL) { 
					SetConsoleTextAttribute(console, 13); 
				}
				else if (level[indexToPrint] == KEY) { 
					SetConsoleTextAttribute(console, 14); 
				}
				else {
					SetConsoleTextAttribute(console, kBaseColor);
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
	switch (input) {
		case 'w':
			newPlayerY--;
		case 'W':
		{
			kPlayerSymbol = 30;
			lookDirection = 1;
			break;
		}
		case 's':
			newPlayerY++;
		case 'S':
		{
			kPlayerSymbol = 31;
			lookDirection = 3;
			break;
		}
		case 'a':
			newPlayerX--;
		case 'A':
		{
			kPlayerSymbol = 17;
			lookDirection = 2;
			break;
		}
		case 'd':
			newPlayerX++;
		case 'D':
		{
			kPlayerSymbol = 16;
			lookDirection = 4;
			break;
		}
		case '\t':
		{
			isMapMode = false;
			isMenuMode = true;
			break;
		}
		case 'e':
		case 'E':
		{
			int actX = playerX;
			int actY = playerY;			
			switch (lookDirection) 
			{
			case 1: /*facing up*/
			{ actY--; break; }
			case 2: /*facing left*/
			{ actX--; break; }
			case 3: /*facing down*/
			{ actY++; break; }
			case 4: /*facing right*/
			{ actX++; break; }
			default: break; 
			}
			//Interact with space player is facing
			int actIndex = GetIndexFromCoordinates(actX, actY, width);
			char interactedWith = level[actIndex];

			if (level[actIndex] == KEY)
			{
				if (DoBinaryAction("COLLECT KEY?", true)) {
					playerHasKey = true;
					level[actIndex] = ' ';
					PlayKeyPickupEffect();
					PrintSubText("KEY COLLECTED");
				}
				else {}
			}
			else if (level[actIndex] == DOR && playerHasKey)
			{
				if (DoBinaryAction("UNLOCK DOOR?", true)) {
					level[actIndex] = ' ';
					playerHasKey = false;
					PlayDoorOpenEffect();
					PrintSubText("DOOR OPENED");
				}
				else {}
			}
			else if (level[actIndex] == DOR && !playerHasKey)
			{
				PlayDoorClosedEffect();
				PrintSubText("DOOR LOCKED");
			}
			break;
		}
		default:
			break;
	}

	//Move player and check for unique space
	int index = GetIndexFromCoordinates(newPlayerX, newPlayerY, width);
	if (level[index] == ' ')
	{
		//Confine player to level edges
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
	else if (level[index] == GOL)
	{
		level[index] = ' ';
		playerX = newPlayerX;
		playerY = newPlayerY;
		return true;
	}
	else {}
	return false;
}

//Action managers
void OpenMenu() 
{
	system("cls");
	cout << endl << " ----[MENU]----" << endl;
	cout << endl << " +-{Inventory}-+";
	if (playerHasKey) { cout << endl << " - KEY"; }
	cout << endl << " +-------------+" << endl;
	cout << endl << " > Map: TAB";
	cout << endl <<" > Quit: ESCAPE" << endl;

	char input = _getch();

	//Exit game
	if(input == 27) {
		if (DoBinaryAction("Really Quit?", false)) {
			gameOver = true;
			continuePlaying = false;
		} else { OpenMenu(); }
	}

	//Menu inputs, will be expanded upon
	else {
		switch (input)
		{
		case '\t':
			isMapMode = true;
			isMenuMode = false;
			break;
		default:
			break;
		}
	}
}
bool DoBinaryAction(string prompt, bool wipe)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (wipe) { system("cls"); }
	cout << endl << "  [" << prompt << "]"; 
	cout << endl << "> ";
	SetConsoleTextAttribute(console, 2);
	cout << "y";
	SetConsoleTextAttribute(console, kBaseColor);
	cout << "es/";
	SetConsoleTextAttribute(console, 4);
	cout << "n";
	SetConsoleTextAttribute(console, kBaseColor);
	cout << "o" << endl;

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
		return DoBinaryAction(prompt, wipe);
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

//Define and print map borders
constexpr char kHorizontalBorder = 205;
constexpr char kTopLeftBorder = 201;
constexpr char kTopRightBorder = 187;
constexpr char kBottomLeftBorder = 200;
constexpr char kBottomRightBorder = 188;
constexpr char kVerticalBorder = 186;
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
