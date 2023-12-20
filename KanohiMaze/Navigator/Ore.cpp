#include "Ore.h"
#include "Player.h"

Ore::Ore(int x, int y)
	: WorldActor(x, y, AColor::Grey, ASymbol::Ore)
	, Item("Ore", ItemType::RESOURCE, Rarity::SCRAP, Element::NONE)
{

}
Ore::~Ore()
{
	delete m_pPosition;
	m_pPosition = nullptr;
}
bool Ore::Interact()
{
	if (m_input.BinaryChoice("COLLECT ORE?")) {
		Item* nOre = new Ore();
		nOre->RollRarity(1);
		Player::GetInstance()->PickupItem(nOre);
		m_IsActive = false;
		Remove();
		return true;
	}
	else { return false; }
}