#include "Chest.h"
#include <iostream>

Chest::Chest(int x, int y, int value)
	: PlacableActor(x, y)
	, m_value(value)
{

}

void Chest::Draw()
{
	std::cout << (char)239;
}