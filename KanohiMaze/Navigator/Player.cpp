#include "Player.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <algorithm>
#include <vector>
#include <sstream>

#include "SaveManager.h"
#include "Level.h"
#include "Key.h"
#include "Ore.h"

using namespace std;

char kPlayerSymbol = 48;
constexpr int kStartingLives = 3;
constexpr unsigned char WAL = (char)219;

Player::Player()
	: playerFacing(Direction::Down)
	, exited(false)
	, menuIsOpen(false)
	, m_WorldActor(WorldActor(0, 0, AColor::Teal, ASymbol::pDown))
{

}
Player::~Player()
{
	ClearInventory();
	/*while (!m_pItems.empty())
	{
		delete m_pItems.back();
		m_pItems.pop_back();
	}*/
}
Player* Player::instance = nullptr;
Player* Player::GetInstance()
{
	if (instance == nullptr) {
		instance = new Player();
	}
	return instance;
}

void Player::SetFacingDirection(Direction pFacing)
{
	playerFacing = pFacing;
	kPlayerSymbol = (int)playerFacing;
	m_WorldActor.SetSymbol(kPlayerSymbol);
}

Item* Player::CreateNewItem(string itemType, string rarity)
{
	Item* nItem = nullptr;

	Rarity iRarity;
	if (rarity == "Scrap")
		iRarity = Rarity::SCRAP;
	else if (rarity == "Decent")
		iRarity = Rarity::DECENT;
	else if (rarity == "Great")
		iRarity = Rarity::GREAT;
	else if (rarity == "Epic")
		iRarity = Rarity::EPIC;
	else if (rarity == "Ancient")
		iRarity = Rarity::ANCIENT;
	else if (rarity == "None")
		iRarity = Rarity::NONE;
	else { iRarity = Rarity::NONE; }

	if (itemType == "Key")
	{
		nItem = new Key();
		PickupItem(nItem);
	}
	else if (itemType == "Ore")
	{
		nItem = new Ore();
		nItem->SetRarity(iRarity);
		PickupItem(nItem);
	}
	else {}

	return nItem;
}

void Player::PickupItem(Item* pItem)
{
	bool exists = false;

	string refItemName = pItem->GetFullName();
	if (m_inventory.size() != 0)
	{
		for (vector<vector<Item*>>::iterator it = m_inventory.begin(); it != m_inventory.end(); ++it)
		{
			string compItemName = (*it)[0]->GetFullName(); 
			if (refItemName == compItemName)
			{
				it->push_back(pItem);
				exists = true;
				break;
			}
		}
	}
	if (!exists)
	{
		vector<Item*> tempVI;
		tempVI.push_back(pItem);
		m_inventory.push_back(tempVI);
	}
}

bool Player::FindKey(bool spendKey)
{
	bool hasKey = false;

	if (m_inventory.size() != 0)
	{
		for (vector<vector<Item*>>::iterator it = m_inventory.begin(); it != m_inventory.end(); ++it)
		{
			string compItemName = (*it)[0]->GetName();
			if (compItemName == "Key")
			{
				if (it->size() > 0)
				{
					if (spendKey == true)
					{
						it->pop_back();
						if (it->empty()){ m_inventory.erase(it); }
					}
					hasKey = true;
					break;
				}
			}
		}
	}
	return hasKey;
}

void Player::ListInventory()
{
	int invSize = 0;
	if (m_inventory.size() != 0)
	{
		cout << endl << endl << "    +---{Inventory}---+";
		for (vector<vector<Item*>>::iterator it = m_inventory.begin(); it != m_inventory.end(); ++it)
		{
			invSize++;
			cout << endl << "    - ";
			cout << "x" << it->size() << " ";
			(*it)[0]->Print();

		}
		cout << endl << "    +-----------------+";
		if (true) {
			m_visuals.ColorText(AColor::White);
			invSize += 1;
			int width;
			width = 21;
			int height;
			height = invSize;
			COORD frameAlign = { (-2),(0) };
			//cout << WAL;
			for (int t = 0; t < width + 2; t++) { m_visuals.DrawAtSpace(t, -1, WAL, frameAlign); }
			//cout << WAL;

			for (int y = 0; y < height + 2; ++y)
			{
				m_visuals.DrawAtSpace(0, y, WAL, frameAlign);
				m_visuals.DrawAtSpace(1, y, WAL, frameAlign);

				m_visuals.DrawAtSpace(width, y, WAL, frameAlign);
				m_visuals.DrawAtSpace(width + 1, y, WAL, frameAlign);
			}

			//cout << WAL;
			for (int b = 0; b < width + 2; b++) { m_visuals.DrawAtSpace(b, height + 1, WAL, frameAlign); }
			//cout << WAL;
		}
		COORD postInv = { (0),(invSize + 4) };
		m_visuals.ResetCursor(postInv);
	}
	else { cout << endl; }
}

void Player::LoadInventory()
{
	for (int i = 0; i < m_itemList.size(); i++)
	{
		int iCount = 0;
		string iRarity;
		string iType;
		string iElement;

		string s, str;
		s = m_itemList[i];
		stringstream ss(s);

		int sIndex = 0;
		while (getline(ss, str, ':'))
		{
			switch (sIndex)
			{
			case 0: iCount = stoi(str);
				break;
			case 1: iRarity = str;
				break;
			case 2: iType = str;
				break;
			case 3: iElement = str;
				break;
			default:
				break;
			}
			sIndex++;
		}

		for (int c = 0; c < iCount; c++)
		{
			Item* testingItem = nullptr;
			testingItem = CreateNewItem(iType, iRarity);
		}
	}
}
void Player::SaveInventory()
{
	if (m_inventory.size() != 0)
	{
		for (vector<vector<Item*>>::iterator it = m_inventory.begin(); it != m_inventory.end(); ++it)
		{
			string iName = (*it)[0]->GetFullName();
			string iCount = to_string(it->size());
			string compressedItem = iCount + ":" + iName;
			m_itemList.push_back(compressedItem);
		}
	}
}
void Player::ClearInventory()
{
	if (m_inventory.size() != 0)
	{
		for (vector<vector<Item*>>::iterator it = m_inventory.begin(); it != m_inventory.end(); ++it)
		{
			while (!it->empty())
			{
				delete it->back();
				it->pop_back();

			}
		}
		m_inventory.clear();
	}
	if (m_itemList.size() != 0)
	{
		m_itemList.clear();
	}

}

void Player::OpenMenu()
{
	menuIsOpen = true;
	do {
		system("cls");

		ListInventory();

		//Menu control prints
		cout << "  ";
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
			{
				SaveManager::GetInstance()->WriteToSaveFile();
				SaveManager::GetInstance()->activeSaveFile = " ";
				exited = true;
				menuIsOpen = false;
			}
		}

		//Menu inputs, will be expanded upon
		else {
			switch (input)
			{
			case '\t':
				menuIsOpen = false;
				Level::GetInstance()->SetDrawnState(false);
				system("cls");
				break;
			default:
				break;
			}
		}
	} while (menuIsOpen);
}

bool Player::HandleMovement()
{
	char input = (char)_getch();
	int newPlayerX = m_WorldActor.GetXPosition();
	int newPlayerY = m_WorldActor.GetYPosition();

	if (input == 'e' || input == 'E') { Interact(); }

	//Restoring correct symbol on space after player moves from it
	static WorldActor* ingressActor;
	ingressActor = Level::GetInstance()->GetActorAtPos(newPlayerX, newPlayerY);
	int xClean = this->m_WorldActor.m_drawPos.X;
	int yClean = this->m_WorldActor.m_drawPos.Y;
	if (ingressActor != nullptr) { ingressActor->Draw(); }
	else { m_visuals.DrawAtSpace(xClean, yClean, ' '); }

	//Movement key inputs
	switch (input)
	{
	case '\t':
		OpenMenu();
		Level::GetInstance()->SetDrawnState(false);
		break;
	case 'w': newPlayerY--;
	case 'W':
	{ SetFacingDirection(Direction::Up); break; }
	case 's': newPlayerY++;
	case 'S':
	{ SetFacingDirection(Direction::Down); break; }
	case 'a': newPlayerX--;
	case 'A':
	{ SetFacingDirection(Direction::Left); break; }
	case 'd': newPlayerX++;
	case 'D':
	{ SetFacingDirection(Direction::Right); break; }
	default: break;
	}

	//Move player and check for unique space
	bool pathableSpace = false;
	ingressActor = Level::GetInstance()->GetActorAtPos(newPlayerX, newPlayerY);

	if (ingressActor != nullptr) {
		bool isActorSolid = ingressActor->GetSolidity();
		if (isActorSolid == true)
		{ pathableSpace = false; }
		else { pathableSpace = true; }
	} else { pathableSpace = true; }

	if (pathableSpace)
	{
		//Confine player to level edges
		if (newPlayerX < 0)
			newPlayerX = 0;
		else if (newPlayerX == Level::GetInstance()->m_width)
			newPlayerX = Level::GetInstance()->m_width - 1;
		if (newPlayerY < 0)
			newPlayerY = 0;
		else if (newPlayerY == Level::GetInstance()->m_height)
			newPlayerY = Level::GetInstance()->m_height - 1;
		m_WorldActor.SetPosition(newPlayerX, newPlayerY);

		//Check for overlapping actor in new space and perform relevant action
		WorldActor* playerOverlapActor = ingressActor;
		if (playerOverlapActor != nullptr)
		{
			if (playerOverlapActor->GetType() == ActorType::Goal)
			{
				Level::GetInstance()->ClearLevel();
				return true;
			}
		}
	}
	return false;
}

void Player::Interact()
{
	int actX = m_WorldActor.GetXPosition();
	int actY = m_WorldActor.GetYPosition();
	switch (playerFacing)
	{
	case Direction::Up:
	{ actY--; break; }
	case Direction::Down:
	{ actY++; break; }
	case Direction::Left:
	{ actX--; break; }
	case Direction::Right:
	{ actX++; break; }
	default: break;
	}
	//Interact with space player is facing
	WorldActor* actRef = Level::GetInstance()->GetActorAtPos(actX, actY);
	if (actRef != nullptr) {
		if (actRef->IsActive())
		{
			bool enactedChange = false;
			enactedChange = actRef->Interact();
			/*system("cls");*/
			if (enactedChange == true)
			{
				Level::GetInstance()->SetDrawnState(false);
			}
		}
	}
	actRef = nullptr;
}