#include "vld.h" /*Memory Leak Debugger*/

#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <vector>
#include <random>

#include "Game.h"
#include "Level.h"
#include "Player.h"
#include "Input.h"
#include "Item.h"

using namespace std;

bool exitedMaze = true;
bool levelComplete = false;

//Temp score variables
int s_Rooms;
int s_Ores;
int s_Keys;
void PrintScores() {
	cout << endl << " [Items Collected]" << endl;
	if (s_Ores > 0) {
		cout << endl << " - x" << s_Ores;
		cout << " ORE" << endl;
	}
	if (s_Keys > 0) {
		cout << endl << " - x" << s_Keys;
		cout << " KEY" << endl;
	}
	cout << endl << " [Rooms Explored: ";
	cout << s_Rooms << "]" << endl << endl;
}

void TestMode()
{
	cout << " [t] - test mode" << endl;
	//Test Mode
	char input = (char)_getch();
	if (input == 't') {
		cout << " [ENTERED TEST MODE]" << endl;
		do{
		input = (char)_getch();
		Item* randomDisc = new Item("Kanoki", ItemType::RESOURCE, Rarity::GREAT);
		(*randomDisc).RollElement();
		(*randomDisc).RollRarity(3);
		(*randomDisc).Print();
		cout << endl;
		} while (input != (char)27); 
		Item pLoot = Item("TempItem", ItemType::RESOURCE, Rarity::NONE);
		pLoot.ListLootData();
	}
}

int main()
{
	if (false) { TestMode(); }

	Game mazeGame;
	cout << " ---{ WELCOME TO KANOHI MAZE }---" << endl;
	system("pause");
	do {

		if (mazeGame.Load())
		{
			while (!levelComplete)
			{
				mazeGame.Run();
				levelComplete = mazeGame.levelEnd;
				exitedMaze = mazeGame.exitedGame;

				//Temp Score Updates
				//Player::GetInstance()->mats;
				//Player::GetInstance()->keys;
				s_Rooms = mazeGame.roomsCleared;

				if (exitedMaze) { break;  }
			}
			levelComplete = false;
		}
	} while (!exitedMaze);
	system("cls");
	cout << endl << " Exiting maze..." << endl;

	//Post-game results?
	PrintScores();

	//Destroy singletons to free up their used memory
	Player::GetInstance()->DestroyPlayer();
	Level::GetInstance()->DestroyLevel();
	
	system("pause");

}
