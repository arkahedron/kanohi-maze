#include "Input.h"
#include <string>
#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

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