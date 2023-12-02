#include "Ore.h"
#include <iostream>

Ore::Ore(int x, int y)
	: PlacableActor(x, y, AColor::Grey)
	, Item("Ore", ItemType::RESOURCE, Rarity::SCRAP)
{

}

void Ore::Draw()
{
	std::cout << (char)239;
}