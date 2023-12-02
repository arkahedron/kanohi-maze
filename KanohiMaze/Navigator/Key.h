#pragma once
#include "PlacableActor.h"
#include "Item.h"

class Key : public PlacableActor, public Item
{
public:
	Key(int x = 0, int y = 0);

	virtual ActorType GetType() override { return ActorType::Key; }
	virtual void Draw() override;
};