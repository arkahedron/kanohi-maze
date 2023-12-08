#include "Ore.h"

Ore::Ore(int x, int y)
	: WorldActor(x, y, AColor::Grey, ASymbol::Ore)
	, Item("Ore", ItemType::RESOURCE, Rarity::SCRAP)
{

}
