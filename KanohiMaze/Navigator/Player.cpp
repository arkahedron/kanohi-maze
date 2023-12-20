#include "Player.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <algorithm>
#include <vector>

#include "Level.h"
#include "Key.h"
#include "Ore.h"

using namespace std;

char kPlayerSymbol = 48;
constexpr int kStartingLives = 3;

Player::Player()
	: playerFacing(Direction::Down)
	, exited(false)
	, menuIsOpen(false)
	, m_WorldActor(WorldActor(0, 0, AColor::Teal, ASymbol::pDown))
{
	
}
Player::~Player()
{
	while (!m_pItems.empty())
	{
		delete m_pItems.back();
		m_pItems.pop_back();
	}
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

Item* Player::CreatePickedItem()
{
	Key nItem;
	PickupItem(&nItem);
	return &nItem;
}

void Player::PickupItem(Item* pItem)
{
	bool exists = false;

	pItem->UpdateFullName();
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
			string compItemName = (*it)[0]->GetFullName();
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
	if (m_inventory.size() != 0)
	{
		for (vector<vector<Item*>>::iterator it = m_inventory.begin(); it != m_inventory.end(); ++it)
		{
			cout << endl << "   - ";
			cout << "x" << it->size() << " ";
			(*it)[0]->Print();
		}
	}
}

void Player::OpenMenu()
{
	menuIsOpen = true;
	do {
		system("cls");
		//cout << endl << "  ------{MENU}------" << endl;

		cout << endl << "  +---{Inventory}---+";
		ListInventory();
		cout << endl << "  +-----------------+" << endl;
		cout.flush();

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

	if (input == 'e' || input == 'E')
	{
		Interact();
	}
	//else
	//{

		//Restoring correct symbol on space after player moves from it
		static WorldActor* ingressActor;
		ingressActor = Level::GetInstance()->GetActorAtPos(newPlayerX, newPlayerY);
		if (ingressActor != nullptr) { ingressActor->Draw(); }
		else { m_visuals.DrawAtSpace(newPlayerX, newPlayerY, ' '); }


		//Movement inputs
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

		if (Level::GetInstance()->IsSpace(newPlayerX, newPlayerY))
		{
			if (ingressActor != nullptr) {
				bool isImpassable = ingressActor->GetSolidity();
				if (isImpassable == true)
				{
					pathableSpace = false;
				}
				else { pathableSpace = true; }
			}
			else { pathableSpace = true; }
		}
		else { pathableSpace = false; }

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
		}
		else if (Level::GetInstance()->IsGoal(newPlayerX, newPlayerY))
		{
			Level::GetInstance()->ClearLevel();
			return true;
		}
		else {}
	//}
	//m_player->Draw();
	//m_visuals.DrawAtSpace(m_player->GetXPosition(), m_player->GetYPosition(), m_player->GoodDraw());
	m_visuals.ResetTextColor();

	if (newPlayerX == m_WorldActor.GetXPosition() && newPlayerY == m_WorldActor.GetYPosition())
	{
		return false;
	}
	//else
	//{
	//	return HandleCollision(newPlayerX, newPlayerY);
	//}

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