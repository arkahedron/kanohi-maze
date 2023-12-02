#include "Player.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>

#include "Key.h"
#include "Ore.h"

using namespace std;

char kPlayerSymbol = 48;
constexpr int kStartingLives = 3;

Player::Player()
	: PlacableActor(0,0, AColor::Teal)
	, playerFacing(Direction::none)
	, exited(false)
	, menuIsOpen(false)
	, keys(0)
	, mats(0)
	, lives(kStartingLives)
{

}
Player::~Player()
{
	/*while (!m_pItems.empty())
	{
		delete m_pItems.back();
		m_pItems.pop_back();
	}*/
}

void Player::SetFacingDirection(Direction pFacing)
{
	playerFacing = pFacing;
	kPlayerSymbol = (int)playerFacing;
}


bool Player::HasKey()
{
	for (auto item = m_pItems.begin(); item != m_pItems.end(); ++item)
	{
		if ((*item)->GetName() == "Key")
		{
			return true;
		}
	}
	return false;
	//return ((keys > 0) ? true : false); 
}
void Player::PickupKey(int amt)
{
	m_pItems.push_back(new Key());
	keys += amt; 
}
void Player::UseKey()
{ 
	for ( auto item = m_pItems.begin(); item != m_pItems.end(); ++item)
	{
		if ((*item)->GetName() == "Key")
		{
			m_pItems.erase(item);
			break;
		}
	}
	keys--; 
}
void Player::PickupMat(int amt)
{
	for (int i = 0; i < amt; i++)
	{	
		Ore* newOre = new Ore();
		newOre->RollRarity(1);
		m_pItems.push_back(newOre);
	}
	
	mats += amt; 
}

void Player::Draw()
{
	m_visuals.ColorText(AColor::Teal);
	cout << kPlayerSymbol;
	m_visuals.ResetTextColor();
}

char Player::GoodDraw()
{
	m_visuals.ColorText(AColor::Teal);
	return kPlayerSymbol;
}

void Player::OpenMenu()
{
	menuIsOpen = true;
	do {
		system("cls");
		cout << endl << "  ----[MENU]----" << endl;

		cout << endl << "  +-{Inventory}-+";

		for (auto item = m_pItems.begin(); item != m_pItems.end(); ++item)
		{
			if((*item)->GetName() == "Key")
			{
				cout << endl << "   - ";
				m_visuals.ColorText(AColor::Yellow);
				(*item)->Print();
				m_visuals.ResetTextColor();
			}
			if ((*item)->GetName() == "Ore")
			{
				cout << endl << "   - ";
				m_visuals.ColorText(AColor::Yellow);
				(*item)->Print();
				m_visuals.ResetTextColor();
			}
		}

		//if (keys > 0) {
		//	cout << endl << " - x" << keys;
		//	m_visuals.ColorText(AColor::Yellow);
		//	cout << " KEY";
		//	m_visuals.ResetTextColor();
		//}
		//if (mats > 0) {
		//	cout << endl << " - x" << mats;
		//	m_visuals.ColorText(AColor::Grey);
		//	cout << " ORE";
		//	m_visuals.ResetTextColor();
		//}


		cout << endl << "  +-------------+" << endl;

		//Menu control prints
		cout << endl << "  ";
		m_visuals.ColorText(AColor::Inverted);
		cout << " >Map: TAB     ";
		m_visuals.ResetTextColor();
		cout << endl << "  ";
		m_visuals.ColorText(AColor::Inverted);
		cout << " >Quit: ESCAPE ";
		m_visuals.ResetTextColor();
		cout << endl;

		char input = _getch();

		//Exit game selection
		if (input == 27) 
		{
			if (m_input.BinaryChoice("REALLY QUIT?"))
			{ exited = true; menuIsOpen = false; }
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