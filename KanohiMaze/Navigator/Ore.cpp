#include "Ore.h"
#include "Player.h"

Ore::Ore(int x, int y)
	: WorldActor(x, y, AColor::Grey, ASymbol::Ore)
	, Item("Ore", ItemType::RESOURCE, Rarity::SCRAP)
{

}

bool Ore::Interact()
{
	if (m_input.BinaryChoice("COLLECT ORE?")) {
		RollRarity(1);
		Player::GetInstance()->PickupItem(this);
		Remove();
		///PlayPickupEffect();
		//m_visuals.SubText("ORE COLLECTED");
		return true;
	}
	else { return false; }
	Update();
}