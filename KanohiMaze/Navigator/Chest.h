#pragma once
#include "WorldActor.h"
class Chest : public WorldActor
{
public:
	Chest(int x, int y);
	virtual ActorType GetType() override { return ActorType::Chest; }

};