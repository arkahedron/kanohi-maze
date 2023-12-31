#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>

/*OLD UTILITY PROGRAM FOR MAP MAKING*/

using namespace std;

constexpr char kCursor = 197;

//Border character definitions
constexpr char kTopRightBorder = 187;
constexpr char kTopLeftBorder = 201;
constexpr char kBottomRightBorder = 188;
constexpr char kBottomLeftBorder = 200;
constexpr char kHorizontalBorder = 205;
constexpr char kVerticalBorder = 186;

constexpr int kArrowInput = 224;
constexpr int kLeftArrow = 75;
constexpr int kRightArrow = 77;
constexpr int kUpArrow = 72;
constexpr int kDownArrow = 80;
constexpr int kEscape = 27;
constexpr int kBackspace = 8;

char mainInput;
int menuPick = 0;
bool continueProgram = true;

void GetLevelDimensions(int& width, int& height);
void DisplayLevel(char* pLevel, int width, int height, int cursorX, int cursorY);
int GetIndexFromXY(int x, int y, int width);

void DisplayTopBorder(int width);
void DisplayBottomBorder(int width);
void DisplayLeftBorder();
void DisplayRightBorder();

string SelectLevelFromList(bool manualSelect);
char* LoadLevel(string levelName, int& width, int& height);
string importedLevel = "templevelname";

bool EditLevel(char* pLevel, int& cursorX, int& cursorY, int width, int height);
void SaveLevel(char* pLevel, int width, int height);

bool BinaryInput(string prompt, bool wipe);

int bufferX, bufferY;
static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
void DrawAtSpace(int x, int y, char thing) 
{	
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	coord.X += bufferX;
	coord.Y += bufferY;
	SetConsoleCursorPosition(hOut, coord);
	cout << thing;
	cout.flush();
	//coord.X--;
	SetConsoleCursorPosition(hOut, coord);
}
void PushToStart()
{
	COORD Start = {0,0};
	//Start.X = 0;
	//Start.Y = 0;
	SetConsoleCursorPosition(hOut, Start);
}



int main()
{
	do {
		cout << " [MENU] " << endl;
		cout << " 1: symbols helper" << endl;
		cout << " 2: create new map" << endl;
		cout << " 3: edit existing map" << endl;
		cout << " 4: exit program" << endl;

		cin >> menuPick;
		switch (menuPick)
		{
		case 1: {
			cout << " Symbols Helper: " << endl;
			for (int i = 1; i < 250; i++)
			{
				char symb = i;
				cout << i << ": " << symb << endl;
			}
			break;
		}
		case 2: {
			int levelWidth;
			int levelHeight;

			GetLevelDimensions(levelWidth, levelHeight);

			system("cls");
			char* pLevel = new char[levelWidth * levelHeight];

			for (int i = 0; i < levelWidth * levelHeight; i++)
			{
				pLevel[i] = ' ';
			}

			int cursorX = 0;
			int cursorY = 0;
			bool doneEditing = false;
			while (!doneEditing)
			{
				//system("cls");
				DisplayLevel(pLevel, levelWidth, levelHeight, cursorX, cursorY);
				doneEditing = EditLevel(pLevel, cursorX, cursorY, levelWidth, levelHeight);
			}

			system("cls");
			DisplayLevel(pLevel, levelWidth, levelHeight, -1, -1);

			SaveLevel(pLevel, levelWidth, levelHeight);

			delete[] pLevel;
			pLevel = nullptr;
			break;
		}
		case 3: {
			int width = 0;
			int height = 0;
			char* levelArray;

			importedLevel = SelectLevelFromList(true);
			levelArray = LoadLevel(importedLevel, width, height);
			system("cls");
			int cursorX = 0;
			int cursorY = 0;
			bool doneEditing = false;
			while (!doneEditing)
			{
				
				DisplayLevel(levelArray, width, height, cursorX, cursorY);
				doneEditing = EditLevel(levelArray, cursorX, cursorY, width, height);
			}

			system("cls");
			DisplayLevel(levelArray, width, height, -1, -1);

			SaveLevel(levelArray, width, height);

			delete[] levelArray;
			levelArray = nullptr;
			break;
		}
		case 4:
		{
			continueProgram = false;
			break;
		}
		default:
			break;
		}
		if (!continueProgram) { cout << endl << " Exiting program..." << endl; break; }
	} while (true);
}


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
						cout << " > " << levelNum << ": " << levelStr << endl;
					}
				}
			} while (FindNextFile(hFind, &FindFileData) != 0);
			FindClose(hFind);

			//Player input to select level, will be overhauled once map tethering is implemented
			string levelInput;
			cin >> levelInput;
			levelInput.append(".txt");
			return levelInput;
		}
		else { cout << " No valid levels found!" << endl; }
	}
	else { return "new"; }
}
char* LoadLevel(string levelName, int& width, int& height)
{
	//Get defined text file as level and try to open it
	levelName.insert(0, "../Levels/");
	ifstream levelFile;
	levelFile.open(levelName);
	if (levelFile)
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
	else {
		cout << " Invalid file, level failed to load!" << endl;
		return LoadLevel(SelectLevelFromList(true), width, height);
	}
}

void SaveLevel(char* pLevel, int width, int height)
{
	string levelName;
	if (menuPick == 3) {
		if (BinaryInput("Save as a new level?", false))
		{
			cout << "Name the level (eg: Level1): ";
			cin >> levelName;
			levelName.append(".txt");
		}
		else {
			if (BinaryInput("Really overwrite existing file?", false))
			{
				cout << "Name the level (eg: Level1): ";
				levelName = importedLevel;
				
			}
			else {
				cout << "Name the level (eg: Level1): ";
				cin >> levelName;
				levelName.append(".txt");
			}
		}
	}
	else {
		cout << "Name the level (eg: Level1): ";
		cin >> levelName;
		levelName.append(".txt");
	}


	levelName.insert(0, "../Levels/");

	ofstream levelFile;
	levelFile.open(levelName);
	if (!levelFile)
	{
		cout << "Opening file failed!" << endl;
	}
	else
	{
		levelFile << width << endl;
		levelFile << height << endl;
		levelFile.write(pLevel, width * height);
		if (!levelFile)
		{
			cout << "Write failed!" << endl;
		}
		levelFile.close();
	}
}
int newCursorX;
int newCursorY;
bool EditLevel(char* pLevel, int& cursorX, int& cursorY, int width, int height)
{
	newCursorX = cursorX;
	newCursorY = cursorY;

	int intInput = _getch();

	if (intInput == kArrowInput)
	{
		int arrowInput = _getch();
		switch (arrowInput)
		{
		case kLeftArrow:
			newCursorX--;
			break;
		case kRightArrow:
			newCursorX++;
			break;
		case kUpArrow:
			newCursorY--;
			break;
		case kDownArrow:
			newCursorY++;
			break;
		}

		if (newCursorX < 0)
			newCursorX = 0;
		else if (newCursorX == width)
			newCursorX = width - 1;
		if (newCursorY < 0)
			newCursorY = 0;
		else if (newCursorY == height)
			newCursorY = height - 1;

		int index = GetIndexFromXY(cursorX, cursorY, width);

		DrawAtSpace(cursorX, cursorY, pLevel[index]);
		cursorX = newCursorX;
		cursorY = newCursorY;
		index = GetIndexFromXY(newCursorX, newCursorY, width);
		DrawAtSpace(newCursorX, newCursorY, kCursor);
	}
	else
	{
		if (intInput == kEscape)
		{
			return true;
		}
		else if (intInput == kBackspace)
		{
			//ignore
		}
		else
		{
			int index = GetIndexFromXY(newCursorX, newCursorY, width);
			pLevel[index] = (char)intInput; 
			DrawAtSpace(cursorX, cursorY, pLevel[index]);
		}
	}
	PushToStart();
	return false;
}

bool BinaryInput(string prompt, bool wipe)
{
	if (wipe) { system("cls"); }
	cout << endl << "  [" << prompt << "]";
	cout << endl << "> ";

	cout << "y/n" << endl;

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

void GetLevelDimensions(int& width, int& height)
{
	cout << "Enter the width of your level: ";
	cin >> width;

	cout << "Enter the height of your level: ";
	cin >> height;

}


bool lvlDrawn(false);

void DisplayLevel(char* pLevel, int width, int height, int cursorX, int cursorY)
{
	if (lvlDrawn == false)
	{
		cout << " [SYMBOL LEGEND]: " << "O,o = WALL | " << "D,d = DOOR | " << "K,k = KEY " << endl;
		cout << " @,P,p = PLAYER | " << "X,x = GOAL | " << "M,m = ITEM | " << "C,c = CHEST " << endl;
		bufferY += 3;
		bufferX += 1;
	}
	DisplayTopBorder(width);

	for (int y = 0; y < height; y++)
	{
		DisplayLeftBorder();
		for (int x = 0; x < width; x++)
		{
			if (cursorX == x && cursorY == y)
			{
				//int index = GetIndexFromXY(x, y, width);
				
				//DrawAtSpace(cursorX, cursorY, pLevel[index]);
				//DrawAtSpace(newCursorX, newCursorY, kCursor);
				//PushToStart();
			}
			else
			{
				if (lvlDrawn == false)
				{
					int index = GetIndexFromXY(x, y, width);
					DrawAtSpace(x, y, pLevel[index]);
					//SetCursorPosition(x, y);
					//cout << pLevel[index];
				}
			}
		}
		DisplayRightBorder();
	}
	DisplayBottomBorder(width);
	lvlDrawn = true;
}

void DisplayTopBorder(int width)
{
	if (lvlDrawn == false)
	{
		cout << kTopLeftBorder;
		for (int i = 0; i < width; i++)
		{
			cout << kHorizontalBorder;
		}
		cout << kTopRightBorder << endl;
	}
	}
void DisplayBottomBorder(int width) 
{
	if (lvlDrawn == false)
	{
		cout << kBottomLeftBorder;
		for (int i = 0; i < width; i++)
		{
			cout << kHorizontalBorder;
		}
		cout << kBottomRightBorder << endl;
	}
	}
void DisplayLeftBorder()
{
	if (lvlDrawn == false)
	{
		cout << kVerticalBorder;
	}
	}
void DisplayRightBorder()
{
	if (lvlDrawn == false)
	{
		cout << " " << kVerticalBorder << endl;
	}
	}

int GetIndexFromXY(int x, int y, int width)
{
	return x + y * width;
}

