#include "Key.h"
#include <iostream>
#include <Windows.h>

Key::Key(int x, int y)
	: PlacableActor(x, y, AColor::Yellow)
	, Item("Key", ItemType::CONSUMABLE, Rarity::NONE)

{

}

void Key::Draw()
{
	std::cout << "~";
}