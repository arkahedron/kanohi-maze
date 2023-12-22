#pragma once
#include "WorldActor.h"
class Wall : public WorldActor
{
public:
	Wall(int x, int y);
	virtual ActorType GetType() override { return ActorType::Wall; }
};

