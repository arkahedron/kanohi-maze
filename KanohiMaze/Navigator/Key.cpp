#include "Key.h"

Key::Key(int x, int y)
	: PlacableActor(x, y, AColor::Yellow, ASymbol::Key)
	, Item("Key", ItemType::CONSUMABLE, Rarity::NONE)

{

}