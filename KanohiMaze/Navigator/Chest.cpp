#include "Chest.h"
#include "Player.h"
#include <iostream>

#include "Item.h"
#include "Key.h"
#include "Ore.h"

using namespace std;

Chest::Chest(int x, int y)
	: WorldActor(x, y, AColor::Orange, ASymbol::Chest)
{
	m_IsSolid = true;
}

bool Chest::Interact()
{
	if (m_input.BinaryChoice("SEARCH CHEST?")) {
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
		Item* nKey = new Key();
		Player::GetInstance()->PickupItem(nKey);
		break;
	}
	case 2:
	{
		cout << "1";
		m_visuals.ColorText(AColor::Grey);
		cout << " ORE";
		Item* nOre = new Ore();
		nOre->RollRarity(3);
		Player::GetInstance()->PickupItem(nOre);
		break;
	}
	case 3:
	{
		cout << "2";
		m_visuals.ColorText(AColor::Grey);
		cout << " ORE";
		for(int i=0; i<2; i++)
		{
			Item* nOre = new Ore();
			nOre->RollRarity(2);
			Player::GetInstance()->PickupItem(nOre);
		}
		break;
	}
	default:
		break;
	}
	m_visuals.ResetTextColor();
	cout << "]" << endl;
}