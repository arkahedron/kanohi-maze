#include "Input.h"
#include <string>
#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

constexpr int colorBase = 7;
constexpr int colorKey = 14;
constexpr int colorMat = 8;
constexpr int colorBox = 6;

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
	m_visuals.ColorText(2);
	cout << "y";
	m_visuals.ColorText(colorBase);
	cout << "es/";
	m_visuals.ColorText(4);
	cout << "n";
	m_visuals.ColorText(colorBase);
	cout << "o ";
	m_mainInput = _getch();
	switch (m_mainInput)
	{
	case 'y':
	case 'Y':
	case 'E':
		return true;
		break;
	case 'n':
	case 'N':
	case 'Q':
		return false;
		break;
	default:
		cout << endl;
		return BinaryChoice(prompt);
		break;
	}
}