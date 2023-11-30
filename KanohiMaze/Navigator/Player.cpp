#include "Player.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;

char kPlayerSymbol = 48; //4
constexpr int kStartingLives = 3;

Player::Player()
	: PlacableActor(0,0, AColor::Teal)
	, lookDirection(0)
	, exited(false)
	, menuIsOpen(false)
	, keys(0)
	, mats(0)
	, lives(kStartingLives)
{

}
Player::~Player()
{

}

void Player::SetLookDirection(int z)
{
	lookDirection = z;
	switch (z) 
	{
	case 1: /*facing up*/
	{ kPlayerSymbol = 65; /*30*/ break; }
	case 2: /*facing down*/
	{ kPlayerSymbol = 86; /*31*/ break; }
	case 3: /*facing left*/
	{ kPlayerSymbol = 60; /*17*/ break; }
	case 4: /*facing right*/
	{ kPlayerSymbol = 62; /*16*/ break; }
	}
}

bool Player::HasKey()
{
	return ((keys > 0) ? true : false);
}
void Player::PickupKey(int amt)
{
	keys += amt;
}
void Player::UseKey()
{
	keys--;
}
void Player::PickupMat(int amt)
{
	mats += amt;
}

void Player::Draw()
{
	m_visuals.ColorText(11);
	cout << kPlayerSymbol;
	m_visuals.ColorText(7);
}

char Player::GoodDraw()
{
	m_visuals.ColorText(11);
	return kPlayerSymbol;
}

void Player::OpenMenu()
{
	menuIsOpen = true;
	do {
		system("cls");
		cout << endl << " ----[MENU]----" << endl;
		cout << endl << " +-{Inventory}-+";

		if (keys > 0) {
			cout << endl << " - x" << keys;
			m_visuals.ColorText(14);
			cout << " KEY";
			m_visuals.ColorText(7);
		}
		if (mats > 0) {
			cout << endl << " - x" << mats;
			m_visuals.ColorText(8);
			cout << " ORE";
			m_visuals.ColorText(7);
		}
		cout << endl << " +-------------+" << endl;
		cout << endl << " >Map: TAB";
		cout << endl << " >Quit: ESCAPE ";

		char input = _getch();

		//Exit game
		if (input == 27) {
			cout << endl;
			if (m_input.BinaryChoice("REALLY QUIT?"))
			{
				exited = true;
				menuIsOpen = false;
			}
			else {}
		}

		//Menu inputs, will be expanded upon
		else {
			switch (input)
			{
			case '\t':
				menuIsOpen = false;
				system("cls");
				break;
			default:
				break;
			}
		}
	} while (menuIsOpen);
}