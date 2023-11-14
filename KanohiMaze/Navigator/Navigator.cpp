#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>

using namespace std;

constexpr char kPlayerSymbol = '@';
constexpr char WAL = 219;
constexpr char KEY = 232;
constexpr char DOR = 177;
constexpr char GOL = 36;
constexpr int kOpenDoorColor = 10;
constexpr int kClosedDoorColor = 12;
constexpr int kRegularColor = 7;

constexpr char kTopRightBorder = 187;
constexpr char kTopLeftBorder = 201;
constexpr char kBottomRightBorder = 188;
constexpr char kBottomLeftBorder = 200;
constexpr char kHorizontalBorder = 205;
constexpr char kVerticalBorder = 186;

int GetIndexFromCoordinates(int x, int y, int width);
void DrawLevel(char level[], int width, int height, int playerX, int playerY, bool playerHasKey);
bool UpdatePlayerPosition(char level[], int& playerX, int& playerY, int width, bool& playerHasKey);

void PlayDoorClosedSound();
void PlayDoorOpenSound();
void PlayKeyPickupSound();
void PlayWinSound();

void DisplayTopBorder(int width);
void DisplayBottomBorder(int width);
void DisplayLeftBorder();
void DisplayRightBorder();

char* LoadLevel(string levelName, int& width, int& height);
bool ConvertLevel(char* level, int width, int height, int& playerX, int& playerY);


int main()
{
	int width = 0;
	int height = 0;

	char* levelArray = LoadLevel("Level1.txt", width, height);

	int playerX = 1;
	int playerY = 1;

	bool anyWarnings = ConvertLevel(levelArray, width, height, playerX, playerY);
	if (anyWarnings)
	{
		cout << "There were some warnings in the level data, see above." << endl;
		system("pause");
	}

	bool playerHasKey = false;
	bool gameOver = false;


	while (!gameOver)
	{
		system("cls");
		DrawLevel(levelArray, width, height, playerX, playerY, playerHasKey);
		gameOver = UpdatePlayerPosition(levelArray, playerX, playerY, width, playerHasKey);
	}
	system("cls");
	DrawLevel(levelArray, width, height, playerX, playerY, playerHasKey);
	cout << "YOU WIN." << endl;
	PlayWinSound();


	delete[] levelArray;
}

char* LoadLevel(string levelName, int& width, int& height)
{
	levelName.insert(0, "../");
	ifstream levelFile;
	levelFile.open(levelName);
	if(!levelFile)
	{
		cout << "Opening file failed!" << endl;
		return nullptr;
	}
	else
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



int GetIndexFromCoordinates(int x, int y, int width)
{
	return x + y * width;
}

void DrawLevel(char level[], int width, int height, int playerX, int playerY, bool playerHasKey)
{
	DisplayTopBorder(width);

	for (int y = 0; y < height; y++)
	{
		DisplayLeftBorder();

		for (int x = 0; x < width; x++)
		{
			if (playerX == x && playerY == y)
			{
				cout << kPlayerSymbol;
			}
			else
			{
				int indexToPrint = GetIndexFromCoordinates(x, y, width);
				HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
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
		default:
			break;
	}

	int index = GetIndexFromCoordinates(newPlayerX, newPlayerY, width);
	if (level[index] == ' ')
	{
		playerX = newPlayerX;
		playerY = newPlayerY;
	}
	else if (level[index] == KEY)
	{
		playerHasKey = true;
		level[index] = ' ';
		playerX = newPlayerX;
		playerY = newPlayerY;
		PlayKeyPickupSound();
	}
	else if (level[index] == DOR && playerHasKey)
	{
		level[index] = ' ';
		playerHasKey = false;
		playerX = newPlayerX;
		playerY = newPlayerY;
		PlayDoorOpenSound();
	}
	else if (level[index] == DOR && !playerHasKey)
	{
		PlayDoorClosedSound();
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

void PlayDoorClosedSound()
{
	Beep(500, 75);
	Beep(500, 75);
}
void PlayDoorOpenSound()
{
	Beep(1397, 200);
}
void PlayKeyPickupSound()
{
	Beep(1568, 200);
}
void PlayWinSound()
{
	Beep(1245, 150);
	Beep(1397, 150);
	Beep(1568, 200);
	Beep(1868, 400);
}


void DisplayTopBorder(int width)
{
	cout << kTopLeftBorder;
	for (int i = 0; i < width; i++)
	{
		cout << kHorizontalBorder;
	}
	cout << kTopRightBorder << endl;
}
void DisplayBottomBorder(int width)
{
	cout << kBottomLeftBorder;
	for (int i = 0; i < width; i++)
	{
		cout << kHorizontalBorder;
	}
	cout << kBottomRightBorder << endl;
}
void DisplayLeftBorder()
{
	cout << kVerticalBorder;
}
void DisplayRightBorder()
{
	cout << kVerticalBorder << endl;
}
