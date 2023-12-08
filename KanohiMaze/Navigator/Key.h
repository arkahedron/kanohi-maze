#pragma once
#include "WorldActor.h"
#include "Item.h"

class Key : public WorldActor, public Item
{
public:
	Key(int x = 0, int y = 0);
	virtual ActorType GetType() override { return ActorType::Key; }
};