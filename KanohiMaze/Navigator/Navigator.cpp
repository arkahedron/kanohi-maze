#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <random>

using namespace std;

//Randomizer core
using u32 = uint_least32_t;
using engine = std::mt19937;
int RNGen(int min, int max);

//Default map initialization
string defaultLevel = "Closet.txt";
string levelName = defaultLevel;
bool manualLevelSelect;
string tempLevelsList[] = { "Cavern.txt", "Closet.txt", "Corner.txt", "Hall.txt", "Shack.txt", "Tree.txt", "Vault.txt" };
int rngLevelPick = 0;
//Custom text flavoring
char kPlayerSymbol = 4;
constexpr char WAL = 219;
constexpr char KEY = 126;
constexpr char MAT = 127;
constexpr char DOR = 177;
constexpr char GOL = 234;
constexpr int colorBase = 7;
constexpr int colorKey = 14;
constexpr int colorMat = 3;
//Border symbols
constexpr char kHorizontalBorder = 205;
constexpr char kTopLeftBorder = 201;
constexpr char kTopRightBorder = 187;
constexpr char kBottomLeftBorder = 200;
constexpr char kBottomRightBorder = 188;
constexpr char kVerticalBorder = 186;
constexpr char kHole = 206;


//Level construction
string SelectLevelFromList(bool manualSelect);
char* LoadLevel(string levelName, int& width, int& height);
bool ConvertLevel(char* level, int width, int height, int& playerX, int& playerY);

//Map gameplay managers
int GetIndexFromCoordinates(int x, int y, int width);
void DrawLevel(char level[], int width, int height, int playerX, int playerY, int keyAmount);
bool UpdatePlayerPosition(char level[], int& playerX, int& playerY, int width, int& keyAmount);

//State effect managers
void PlayDoorClosedEffect();
void PlayDoorOpenEffect();
void PlayPickupEffect();
void PlayWinEffect();

//Border constructors
void DisplayTopBorder(int width);
void DisplayBottomBorder(int width);
void DisplayLeftBorder();
void DisplayRightBorder();

//Action managers
bool BinaryInput(string prompt, bool wipe);
void PrintMainText(string mainText);
void PrintSubText(string subText);
void OpenMenu();
int BuildDoorway();


//Global variable initializers
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
char mainInput = ' ';
bool inMapMode = false;
string mainText = " ";
string subText = " ";

//Size and assign level map
ifstream activeLevelFile;
int width = 0;
int height = 0;
char* levelArray;
int levelsCleared = 0;
int thisLevelType;
int nextLevelType;


//Initialize map variables
int playerX = 1;
int playerY = 1;
bool levelComplete = false;
bool isMapMode = true;
bool isMenuMode = false;
bool continuePlaying = true;
int lookDirection = 0;
int matAmount = 0;
int keyAmount = 0;
int holeX;
int holeY;


//Main code body
int main()
{
	cout << " ---{ WELCOME TO KANOHI MAZE }---" << endl;

	manualLevelSelect = BinaryInput("Select Level Manually?", false);
	levelName = SelectLevelFromList(manualLevelSelect);

	do {
		if (levelsCleared > 0) 
		{
			rngLevelPick = RNGen(0, size(tempLevelsList));
			levelName = tempLevelsList[rngLevelPick];
			PrintMainText("YOU ENTER A NEW ROOM");

			/* "check rng list and post upcoming level" 
			cout << endl << "RNG:" << size(tempLevelsList) << "::" << rngLevelPick << levelName << endl; 
			system("pause"); */

		} else { PrintMainText("YOU ENTER THE MAZE"); }


		//Select, import, and convert level, then check for errors
		levelArray = LoadLevel(levelName, width, height);
		bool anyWarnings = ConvertLevel(levelArray, width, height, playerX, playerY);
		if (anyWarnings)
		{
			cout << "There were some warnings in the level data, see above." << endl;
			system("pause");
		}

		//Main game Loop
		while (!levelComplete)
		{
			if (isMapMode)
			{
				system("cls");
				DrawLevel(levelArray, width, height, playerX, playerY, keyAmount);
				cout << " > Move: WASD" << " |";
				cout << " > Look: SHIFT+WASD " << endl;
				cout << " > Act: E" << "     |";
				//? cout << " > Attack: Q" << endl;
				cout << " > Menu: TAB ";

				levelComplete = UpdatePlayerPosition(levelArray, playerX, playerY, width, keyAmount);
			}
			else if (isMenuMode)
			{
				OpenMenu();
			}
			else {}
			if (!continuePlaying) { break; }
		}
		if (!continuePlaying) { break; }

		delete[] levelArray;
		activeLevelFile.close();
		levelsCleared++;
		levelComplete = false;

	} while (continuePlaying);
	cout << " Exiting maze..." << endl;

	//Post-game results?
	if (matAmount > 0) {
		cout << endl << " [Materials Collected]" << endl;
		cout << endl << " - x" << matAmount;
		SetConsoleTextAttribute(console, colorMat);
		cout << " ORE" << endl;
		SetConsoleTextAttribute(console, colorBase);
	}
	system("pause");
}

//RNG core
int RNGen(int min, int max)
{
	int randResult;
	random_device os_seed;
	const u32 seed = os_seed();
	engine generator(seed);
	uniform_int_distribution< u32 > distribute(min, max);
	randResult = distribute(generator);

	return randResult;
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
							levelStr.erase(levelStr.find_last_of("."), string::npos);
							levelNum++;
							cout << " > " << levelNum << ": " << levelStr << "\n";
						}
					}
				} while (FindNextFile(hFind, &FindFileData) != 0);
			FindClose(hFind);

			//Player input to select level, will be overhauled once map tethering is implemented
			string levelInput;
			cin >> levelInput;
			levelInput.append(".txt");
			return levelInput;
		} else { cout << " No valid levels found!" << endl; }
	} else { return defaultLevel; }
}
char* LoadLevel(string levelName, int& width, int& height)
{
	//Get defined text file as level and try to open it
	levelName.insert(0, "../Levels/");

	activeLevelFile.open(levelName);
	if(activeLevelFile)
	{
		constexpr int tempSize = 25;
		char temp[tempSize];

		activeLevelFile.getline(temp, tempSize, '\n');
		width = atoi(temp);

		activeLevelFile.getline(temp, tempSize, '\n');
		height = atoi(temp);

		char* levelData = new char[width * height];
		activeLevelFile.read(levelData, width * height);
		return levelData;
	}
	else {
		cout << " Invalid file, level failed to load!" << endl;
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

			//Level conversion key
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
				case 'M':
				case 'm':
					level[index] = MAT;
					break;
				case 'X':
				case 'x':
					level[index] = GOL;
					holeY = y;
					holeX = x;
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
//Draw map body
void DrawLevel(char level[], int width, int height, int playerX, int playerY, int keyAmount)
{
	//Push print body down one and print top border
	cout << "\n";
	DisplayTopBorder(width);

	for (int y = 0; y < height; y++)
	{
		//Reset coloration and push print body to right by a space
		SetConsoleTextAttribute(console, colorBase);
		cout << "  ";

		//Print LEFT border and check for goal on its edge to place hole
		if (holeX == 0 && holeY == y) { cout << kHole;}
		else { DisplayLeftBorder(); }

		for (int x = 0; x < width; x++)
		{
			if (playerX == x && playerY == y) {
				//Colorize and print player symbol
				SetConsoleTextAttribute(console, 11);
				cout << kPlayerSymbol;
				SetConsoleTextAttribute(console, colorBase);
			}
			else
			{
				int indexToPrint = GetIndexFromCoordinates(x, y, width);
				//Colorize other unique symbols
				if (level[indexToPrint] == DOR) {
					if (keyAmount>0) { SetConsoleTextAttribute(console, 2); }
					else{ SetConsoleTextAttribute(console, 4); }
				}
				else if (level[indexToPrint] == MAT) { SetConsoleTextAttribute(console, colorMat); }
				else if (level[indexToPrint] == GOL) { SetConsoleTextAttribute(console, 13); }
				else if (level[indexToPrint] == KEY) { SetConsoleTextAttribute(console, colorKey); }
				else { SetConsoleTextAttribute(console, colorBase); }
				//Print relevant symbol and reset color to base
				cout << level[indexToPrint];
				SetConsoleTextAttribute(console, colorBase);
			}
		}
		//Print RIGHT border and check for goal on its edge to place hole
		if (holeX == width-1 && holeY == y) { cout << kHole << endl; }
		else { DisplayRightBorder(); }
		
	}
	//Print BOTTOM border
	DisplayBottomBorder(width);
}
//Map movement and action check
bool UpdatePlayerPosition(char level[], int& playerX, int& playerY, int width, int& keyAmount)
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
				if (BinaryInput("COLLECT KEY?", true)) {
					keyAmount++;
					level[actIndex] = ' ';
					PlayPickupEffect();
					PrintSubText("KEY COLLECTED");
				}
				else {}
			}
			else if (level[actIndex] == MAT)
			{
				if (BinaryInput("COLLECT ORE?", true)) {
					matAmount++;
					level[actIndex] = ' ';
					PlayPickupEffect();
					PrintSubText("ORE COLLECTED");
				}
				else {}
			}
			else if (level[actIndex] == DOR && keyAmount>0)
			{
				if (BinaryInput("UNLOCK DOOR?", true)) {
					level[actIndex] = ' ';
					keyAmount--;
					PlayDoorOpenEffect();
					PrintSubText("DOOR OPENED");
				}
				else {}
			}
			else if (level[actIndex] == DOR && keyAmount<1)
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

	if (keyAmount) {
		cout << endl << " - x" << keyAmount;
		SetConsoleTextAttribute(console, colorKey);
		cout << " KEY";
		SetConsoleTextAttribute(console, colorBase);
	}
	if (matAmount > 0) { 
		cout << endl << " - x" << matAmount;
		SetConsoleTextAttribute(console, colorMat);
		cout << " ORE";
		SetConsoleTextAttribute(console, colorBase);
	}
	cout << endl << " +-------------+" << endl;
	cout << endl << " > Map: TAB";
	cout << endl << " > Quit: ESCAPE" << endl;

	char input = _getch();

	//Exit game
	if(input == 27) {
		if (BinaryInput("Really Quit?", false)) {
			levelComplete = true;
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
bool BinaryInput(string prompt, bool wipe)
{
	if (wipe) { system("cls"); }
	cout << endl << "  [" << prompt << "]"; 
	cout << endl << "> ";
	SetConsoleTextAttribute(console, 2);
	cout << "y";
	SetConsoleTextAttribute(console, colorBase);
	cout << "es/";
	SetConsoleTextAttribute(console, 4);
	cout << "n";
	SetConsoleTextAttribute(console, colorBase);
	cout << "o" << endl;

	mainInput = _getch();

	switch (mainInput)
	{
	case 'y':
	case 'Y':
		return true;
		break;
	case 'n':
	case 'N':
		return false;
		break;
	default:
		return BinaryInput(prompt, wipe);
		break;
	}
}
void PrintMainText(string mainText) {
	cout << " " << endl;
	system("cls");
	cout << endl << "  [" << mainText << "]" << endl;
	system("pause");
}
void PrintSubText(string subText)
{
	cout << " " << endl;
	system("cls");
	DrawLevel(levelArray, width, height, playerX, playerY, keyAmount);
	cout << "  [" << subText << "]" << endl;
	system("pause");
}

//Play state effects
void PlayDoorClosedEffect()
{
	/*
	Beep(400, 75);
	Beep(300, 75);
	*/
}
void PlayDoorOpenEffect()
{
	/*
	Beep(900, 200);
	*/
}
void PlayPickupEffect()
{	
	/*
	Beep(600, 200);
	*/
}
void PlayWinEffect()
{
	/*
	Beep(900, 150);
	Beep(1100, 150);
	Beep(1300, 200);
	Beep(1600, 400);
	*/
}

//Print map borders
void DisplayTopBorder(int width)
{
	SetConsoleTextAttribute(console, colorBase);
	cout << "  " << kTopLeftBorder;
	for (int i = 0; i < width; i++)
	{
		//Goal on TOP edge hole check and place
		if (holeY == 0 && holeX == i) { cout << kHole; }
		else { cout << kHorizontalBorder; }
	}
	cout << kTopRightBorder << endl;
}
void DisplayBottomBorder(int width)
{
	SetConsoleTextAttribute(console, colorBase);
	cout << "  " << kBottomLeftBorder;
	for (int i = 0; i < width; i++)
	{
		//Goal on BOTTOM edge hole check and place
		if (holeY == height-1 && holeX == i) { cout << kHole; }
		else { cout << kHorizontalBorder; }
	}
	cout  << kBottomRightBorder << endl;
}
void DisplayLeftBorder()
{
	SetConsoleTextAttribute(console, colorBase);
	cout << kVerticalBorder;
}
void DisplayRightBorder()
{
	SetConsoleTextAttribute(console, colorBase);
	cout << kVerticalBorder << endl;
}

int rSeed = -1;
int BuildDoorway()
{
	if (rSeed == -1) 
	{
		rSeed = RNGen((width / 3), width - (width / 3));
	}
	return rSeed;
}