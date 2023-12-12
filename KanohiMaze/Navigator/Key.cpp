#include "Key.h"
#include "Player.h"

Key::Key(int x, int y)
	: WorldActor(x, y, AColor::Yellow, ASymbol::Key)
	, Item("Key", ItemType::CONSUMABLE, Rarity::NONE)

{

}

void Key::Interact()
{
	if (m_input.BinaryChoice("COLLECT KEY?")) {
		Player::GetInstance()->PickupKey(1);
		m_IsActive = false;
		Remove();
		///PlayPickupEffect();
		//m_visuals.SubText("KEY COLLECTED");

	}
	else {}
	Update();
}