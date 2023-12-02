#include "Chest.h"
#include <iostream>

Chest::Chest(int x, int y)
	: PlacableActor(x, y)
{

}

void Chest::Draw()
{
	std::cout << (char)239;
}