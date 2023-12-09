#include "Player.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <algorithm>
#include <vector>

#include "Key.h"
#include "Ore.h"

using namespace std;

char kPlayerSymbol = 48;
constexpr int kStartingLives = 3;

Player::Player()
	: playerFacing(Direction::Down)
	, exited(false)
	, menuIsOpen(false)
	, keys(0)
	, mats(0)
	, lives(kStartingLives)
	, m_WorldActor(WorldActor(0, 0, AColor::Teal, ASymbol::pDown))
{

}
Player* Player::instance = nullptr;
Player* Player::GetInstance()
{
	if (instance == nullptr) {
		instance = new Player();
	}
	return instance;
}
Player::~Player()
{
//	while (!m_pItems.empty())
//	{
//		delete m_pItems.back();
//		m_pItems.pop_back();
//	}
}


void Player::SetFacingDirection(Direction pFacing)
{
	playerFacing = pFacing;
	kPlayerSymbol = (int)playerFacing;
	m_WorldActor.SetSymbol(kPlayerSymbol);
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


char Player::GoodDraw()
{
	m_visuals.ColorText(AColor::Teal);
	return kPlayerSymbol;
}

//void Player::ListInventory() 
//{
//	for (auto item = m_pItems.begin(); item != m_pItems.end(); ++item)
//	{
//		if ((*item)->GetName() == "Key")
//		{
//			cout << endl << "   - ";
//			m_visuals.ColorText(AColor::Yellow);
//			(*item)->Print();
//			m_visuals.ResetTextColor();
//		}
//		if ((*item)->GetName() == "Ore")
//		{
//			cout << endl << "   - ";
//			m_visuals.ColorText(AColor::Yellow);
//			(*item)->Print();
//			m_visuals.ResetTextColor();
//		}
//	}
//}

void Player::ListInventory()
{
	//vector<std::string> uniqueItems;
	//for (auto item = m_pItems.begin(); item != m_pItems.end(); ++item)
	//{
	//	string uItem = (*item)->GetFullName();
	//	uniqueItems.push_back(uItem);
	//}
	//std::sort(uniqueItems.begin(), uniqueItems.end());
	//auto last = std::unique(m_pItems.begin(), m_pItems.end());
	////uniqueItems.erase(last, uniqueItems.end());
	//for (auto item = uniqueItems.begin(); item != uniqueItems.end(); ++item)
	//{
	//	int iCount = std::count(uniqueItems.begin(), uniqueItems.end(), item);
	//	cout << " - x" << iCount;
	//	//(*item)->Print();
	//}

	for (auto item = m_pItems.begin(); item != m_pItems.end(); ++item)
	{
	
			cout << endl << "   - ";
			(*item)->Print();
	}

}

void Player::OpenMenu()
{
	menuIsOpen = true;
	do {
		system("cls");
		cout << endl << "  ----[MENU]----" << endl;

		cout << endl << "  +-{Inventory}-+";
		ListInventory();
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