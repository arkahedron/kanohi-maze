#include "Key.h"
#include "Player.h"

Key::Key(int x, int y)
	: WorldActor(x, y, AColor::Yellow, ASymbol::Key)
	, Item("Key", ItemType::CONSUMABLE, Rarity::NONE)

{

}

bool Key::Interact()
{
	if (m_input.BinaryChoice("COLLECT KEY?")) {
		Player::GetInstance()->PickupItem(this);
		Remove();
		///PlayPickupEffect();
		//m_visuals.SubText("KEY COLLECTED");
		return true;
	}
	else { return false; }
	Update();
}