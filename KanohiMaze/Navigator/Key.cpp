#include "Key.h"

Key::Key(int x, int y)
	: WorldActor(x, y, AColor::Yellow, ASymbol::Key)
	, Item("Key", ItemType::CONSUMABLE, Rarity::NONE)

{

}