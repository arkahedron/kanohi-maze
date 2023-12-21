#include "Input.h"
#include <string>
#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

constexpr unsigned int kEscapeKey = 27;
constexpr unsigned int kEnterKey = 13;
constexpr unsigned int kUpKey = 72;
constexpr unsigned int kDownKey = 80;

constexpr unsigned char WAL = (char)219;

Input::Input()
	: m_mainInput(' ')
{

}
Input::~Input()
{

}

bool Input::BinaryChoice(string prompt)
{
	cout  << " [" << prompt << "]";
	cout << endl << " > ";
	m_visuals.ColorText(AColor::DarkGreen);
	cout << "y";
	m_visuals.ResetTextColor();
	cout << "es/";
	m_visuals.ColorText(AColor::DarkRed);
	cout << "n";
	m_visuals.ResetTextColor();
	cout << "o ";
	m_mainInput = _getch();
	switch (m_mainInput)
	{
	case 'y':
	case 'Y':
	case 'E':
		m_visuals.WipeLastLines(1);
		return true;
		break;
	case 'n':
	case 'N':
	case 'Q':
		m_visuals.WipeLastLines(1);
		return false;
		break;
	default:
		cout << endl;
		m_visuals.WipeLastLines(2);
		return BinaryChoice(prompt);
		break;
	}
}

string Input::VerticalMenu(vector<string> stringList, COORD buffer, bool enclose)
{
	vector<string> MenuList = stringList;
	int lMax = MenuList.size();
	int pick = 0;

	//while (true)
	//{
		//system("cls");
		//m_visuals.ResetTextColor();
		//m_visuals.ResetCursor();
		//cout << endl;
		if (enclose) { cout << "  " << string(15, WAL) << endl; }

		for (int i = 0; i < lMax; ++i)
		{
			//cout << "  ";
			if (enclose) { cout << string(2, WAL); }
			if (i == pick) {
				//m_visuals.ColorText(AColor::Inverted);
				//cout << ">" << MenuList[i] << " ";
				COORD selectPos = buffer;
				selectPos.Y += i;
				m_visuals.ResetCursor(selectPos);
				cout << " >";
			}
			else {
				//m_visuals.ResetTextColor();
				//cout << " " << MenuList[i] << " ";
			}
			m_visuals.ResetTextColor();
			if (enclose) { cout << string(2, WAL); }
			//cout << endl;
		}
		if (enclose) { cout << "  " << string(15, WAL) << endl; }

		while (true)
		{
			int input = _getch();
			if (input == 'w' || (char)input == kUpKey)
			{
				pick -= 1;
				if (pick == -1) { pick = lMax; }
				break;
			}
			else if (input == 's' || (char)input == kDownKey)
			{
				pick += 1;
				if (pick == lMax+1) { pick = 0; }
				break;
			}
			else if (input == 'e' || input == kEnterKey)
			{
				m_visuals.ResetCursor();
				if (BinaryChoice("Confirm Action?")) { return MenuList[pick]; }
				break;
				cout << "\r";

				//cout << "\r";
				/*switch (pick)
				{
				case 0:
					if (BinaryChoice(" ")) { return "0"; }
					break;
				case 1:
					if (BinaryChoice("Start New Game?")) { return  "1"; }
					break;
				case 2:
					if (BinaryChoice("Really Quit?")) { return  "2"; }
					break;
				default:
					break;
				}*/
			}
		}
		Sleep(120);
		m_visuals.WipeLastLines(lMax);
	//}
	return " ";


}