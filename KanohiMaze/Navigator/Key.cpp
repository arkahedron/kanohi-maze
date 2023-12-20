#include "Key.h"
#include "Player.h"

Key::Key(int x, int y)
	: WorldActor(x, y, AColor::Yellow, ASymbol::Key)
	, Item("Key", ItemType::CONSUMABLE)
{

}
Key::~Key()
{
	delete m_pPosition;
	m_pPosition = nullptr;
}

bool Key::Interact()
{
	if (m_input.BinaryChoice("COLLECT KEY?")) {
		Item* nKey = new Key();
		Player::GetInstance()->PickupItem(nKey);
		m_IsActive = false;
		Remove();
		return true;
	}
	else { return false; }
}