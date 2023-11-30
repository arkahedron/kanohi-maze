#pragma once
#include "Player.h"
//#include "Inventory.h"
//#include "Visuals.h"

class Randomizer
{
	//Visuals m_visuals;

public:
	Randomizer();
	~Randomizer();

	int Generate(int min, int max);

	//void ChestLoot(Player pRef);
};