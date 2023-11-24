#include "Menu.h"
#include <string>
#include <iostream>
#include <conio.h>

using namespace std;

Menu::Menu()
	: m_isOpen(false)
{
	
}
Menu::~Menu()
{

}

bool Menu::Open(Inventory inv)
{
	m_isOpen = true;
	do {




	system("cls");
	cout << endl << " ----[MENU]----" << endl;
	cout << endl << " +-{Inventory}-+";

	if (inv.keys > 0) {
		cout << endl << " - x" << inv.keys;
		m_visuals.ColorText(14);
		cout << " KEY";
		m_visuals.ColorText(7);
	}
	if (inv.mats > 0) {
		cout << endl << " - x" << inv.mats;
		m_visuals.ColorText(8);
		cout << " ORE";
		m_visuals.ColorText(7);
	}
	cout << endl << " +-------------+" << endl;
	cout << endl << " >Map: TAB";
	cout << endl << " >Quit: ESCAPE ";

	char input = _getch();

	//Exit game
	if(input == 27) {
		cout << endl;
		if (m_input.BinaryChoice("REALLY QUIT?")) 
		{
			m_isOpen = false;
			return true;
		} 
		else { }
	}

	//Menu inputs, will be expanded upon
	else {
		switch (input)
		{
		case '\t':
			m_isOpen = false;
			break;
		default:
			break;
		}
	}
	} while (m_isOpen);
	return false;
}
