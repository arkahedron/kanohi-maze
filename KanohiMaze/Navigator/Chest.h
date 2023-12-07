#pragma once
#include "PlacableActor.h"
class Chest : public PlacableActor
{
public:
	Chest(int x, int y);
	virtual ActorType GetType() override { return ActorType::Chest; }

};