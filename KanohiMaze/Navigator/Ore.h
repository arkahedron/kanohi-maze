#pragma once
#include "PlacableActor.h"
#include "Item.h"

class Ore : public PlacableActor, public Item
{
public:
	Ore(int x = 0, int y = 0);

	virtual ActorType GetType() override { return ActorType::Ore; }

};