#include "Ore.h"

Ore::Ore(int x, int y)
	: PlacableActor(x, y, AColor::Grey, ASymbol::Ore)
	, Item("Ore", ItemType::RESOURCE, Rarity::SCRAP)
{

}
