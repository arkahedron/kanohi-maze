#include "MainMenuState.h"

#include <iostream>
#include <conio.h>

#include "StateMachineCore.h"

using namespace std;

constexpr int kEscapeKey = 27;

constexpr char kLoadSave = '1';
constexpr char kNewSave = '2';
constexpr char kQuit = '3';

MainMenuState::MainMenuState(StateMachineCore* pOwner)
	: m_pOwner(pOwner)
{

}

bool MainMenuState::Update(bool processInput)
{
	bool shouldQuit = false;
	if (processInput)
	{
		int input = _getch();
		if (input == kEscapeKey || (char)input == kQuit)
		{
			shouldQuit = true;
		}
		else if ((char)input == kNewSave)
		{
			m_pOwner->LoadScene(StateMachineCore::SceneName::Archives);
		}
		//input = NULL;
	}
	
	return shouldQuit;
}

void MainMenuState::Draw()
{
	m_visuals.ResetCursor();
	system("cls");
	cout << endl;
	cout << "       - - - MAIN MENU - - -       " << endl << endl;
	cout << "         1 - LOAD SAVE -           " << endl;
	cout << "         2 - NEW  SAVE -           " << endl;
	cout << "         3 - QUIT GAME -           " << endl;
}