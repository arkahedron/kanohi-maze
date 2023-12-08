#pragma once
#include "WorldActor.h"
class Goal : public WorldActor
{
public:
	Goal(int x, int y);
	virtual ActorType GetType() override { return ActorType::Goal; }
};