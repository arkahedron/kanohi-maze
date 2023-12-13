#include "Chest.h"
#include "Player.h"
#include <iostream>

using namespace std;

Chest::Chest(int x, int y)
	: WorldActor(x, y, AColor::Orange, ASymbol::Chest)
{
	m_IsSolid = true;
}

bool Chest::Interact()
{
	if (m_input.BinaryChoice("SEARCH CHEST?")) {
		//random items?
		GenLoot();
		///PlayPickupEffect();
		Remove();
		return true;
	}
	else { return false; }
	Update();
}

void Chest::GenLoot() {
	//loot table: 1 key, 1 ore, 2 ore

	cout << "\r\33[2K" << " [FOUND x";
	switch (int chestRoll = m_randomizer.Generate(1, 3))
	{
	case 1:
	{
		cout << "1";
		m_visuals.ColorText(AColor::Yellow);
		cout << " KEY";
		m_visuals.ResetTextColor();
		Player::GetInstance()->PickupKey(1);
		break;
	}
	case 2:
	{
		cout << "1";
		m_visuals.ColorText(AColor::Grey);
		cout << " ORE";
		m_visuals.ResetTextColor();
		Player::GetInstance()->PickupMat(1);
		break;
	}
	case 3:
	{
		cout << "2";
		m_visuals.ColorText(AColor::Grey);
		cout << " ORE";
		m_visuals.ResetTextColor();
		Player::GetInstance()->PickupMat(2);
		break;
	}
	default:
		break;
	}
	cout << "]" << endl;
	//m_visuals.WipeLastLines(1);
}