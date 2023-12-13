#pragma once
#include "WorldActor.h"
#include "Randomizer.h"

class Chest : public WorldActor
{
public:
	Chest(int x, int y);
	virtual ActorType GetType() override { return ActorType::Chest; }

	bool Interact() override;

private:
	Randomizer m_randomizer;
	void GenLoot();
};