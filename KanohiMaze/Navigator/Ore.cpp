#include "Ore.h"
#include "Player.h"

Ore::Ore(int x, int y)
	: WorldActor(x, y, AColor::Grey, ASymbol::Ore)
	, Item("Ore", ItemType::RESOURCE, Rarity::SCRAP, Element::NONE)
{

}

bool Ore::Interact()
{
	if (m_input.BinaryChoice("COLLECT ORE?")) {
		Item* nOre = new Ore();
		nOre->RollRarity(1);
		Player::GetInstance()->PickupItem(nOre);
		//Player::GetInstance()->PickupItem(this);
		Remove();
		///PlayPickupEffect();
		//m_visuals.SubText("ORE COLLECTED");
		return true;
	}
	else { return false; }
	Update();
}