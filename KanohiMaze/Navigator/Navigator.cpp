#include "vld.h" /*Memory Leak Debugger*/

#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <vector>
#include <random>

#include "Game.h"
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

	Game myGame;
	cout << " ---{ WELCOME TO KANOHI MAZE }---" << endl;
	system("pause");
	do {

		if (myGame.Load())
		{
			while (!levelComplete)
			{
				myGame.Run();
				levelComplete = myGame.levelEnd;
				exitedMaze = myGame.exitedGame;

				//Temp Score Updates
				//Player::GetInstance()->mats;
				//Player::GetInstance()->keys;
				s_Rooms = myGame.roomsCleared;

				if (exitedMaze) { break;  }
			}
			levelComplete = false;
		}
	} while (!exitedMaze);
	system("cls");
	cout << endl << " Exiting maze..." << endl;
	////Post-game results?
	PrintScores();

	system("pause");

}




////Play state effects
//void PlayDoorClosedEffect()
//{
//	/*
//	Beep(400, 75);
//	Beep(300, 75);
//	*/
//}
//void PlayDoorOpenEffect()
//{
//	/*
//	Beep(900, 200);
//	*/
//}
//void PlayPickupEffect()
//{	
//	/*
//	Beep(600, 200);
//	*/
//}
//void PlayWinEffect()
//{
//	/*
//	Beep(900, 150);
//	Beep(1100, 150);
//	Beep(1300, 200);
//	Beep(1600, 400);
//	*/
//}