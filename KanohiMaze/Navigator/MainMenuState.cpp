#include "MainMenuState.h"

#include <iostream>
#include <conio.h>

#include "StateMachineCore.h"

using namespace std;

constexpr unsigned int kEscapeKey = 27;
constexpr unsigned int kEnterKey = 13;
constexpr unsigned int kUpKey = 72;
constexpr unsigned int kDownKey = 80;

constexpr char kLoadSave = '1';
constexpr char kNewSave = '2';
constexpr char kQuit = '3';

constexpr unsigned char WAL = (char)219;

MainMenuState::MainMenuState(StateMachineCore* pOwner)
	: m_pOwner(pOwner)
{

}

bool MainMenuState::Update(bool processInput)
{
	bool shouldQuit = false;
	int menuSelection = -1;
	if (processInput)
	{
		menuSelection = Selector();

		switch (menuSelection)
		{
		case 0: /*Load Save*/
		{
			m_visuals.WipeLastLines(0);
			cout << endl << "Loading Save...";
			cout << endl << "\r No valid saves found!" << endl;
			system("pause");
		} break;
		case 1: /*New Game*/
		{
			m_visuals.WipeLastLines(0);
			cout << endl << "Starting New Game..." << endl;
			system("pause");
			m_pOwner->LoadScene(StateMachineCore::SceneName::Archives);
		} break;
		case 2: /*Exit Game*/
		{
			m_visuals.WipeLastLines(0);
			cout << endl << "Exiting Game..." << endl;
			shouldQuit = true;
		} break;
		default:
			break;
		}
	}
	
	return shouldQuit;
}

void MainMenuState::Draw()
{

}

int MainMenuState::Selector()
{
	string Menu[3] = { "Load Save", "New  Game", "Exit Game" };
	int pick = 0;
	int selectedOption = pick;
	bool hasSelected = false;
	while (!hasSelected)
	{
		system("cls");
		m_visuals.ResetTextColor();
		m_visuals.ResetCursor();
		cout << "\n";
		cout << "  " << string(15, WAL) << endl;
		cout << "  " << WAL << "[KANOHI MAZE]" << WAL << endl;
		cout << "  " << string(15, WAL) << endl;
		cout << "  " << string(2, WAL) << " MAIN MENU " << string(2, WAL) << endl;
		cout << "  " << string(15, WAL) << endl;
		cout << "  " << string(15, WAL) << endl;

		for (int i = 0; i < 3; ++i)
		{
			cout << "  " << string(2, WAL);
			if (i == pick) {
				m_visuals.ColorText(AColor::Inverted);
				cout << ">" << Menu[i] << " ";
			}
			else {
				m_visuals.ResetTextColor();
				cout << " " << Menu[i] << " ";
			}
			m_visuals.ResetTextColor();
			cout << string(2, WAL) << endl;
		}
		cout << "  " << string(15, WAL) << endl;

		while (true)
		{
			int input = _getch();
			if (input == 'w' || (char)input == kUpKey)
			{
				pick -= 1;
				if (pick == -1) { pick = 2; }
				break;
			}
			else if (input == 's' || (char)input == kDownKey)
			{
				pick += 1;
				if (pick == 3) { pick = 0; }
				break;
			}
			else if (input == 'e' || input == kEnterKey)
			{
				//cout << "\r";
				switch (pick)
				{
				case 0:
					if (m_input.BinaryChoice("Load Existing Save?")) { return 0; }
					break;
				case 1:
					if (m_input.BinaryChoice("Start New Game?")) { return 1; }
					break;
				case 2:
					if (m_input.BinaryChoice("Really Quit?")) { return 2; }
					break;
				default:
					break;
				}
				cout << "\r";
			}
		}
		Sleep(120);
	}
	return -1;
}