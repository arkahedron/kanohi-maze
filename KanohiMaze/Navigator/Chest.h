#pragma once
#include "PlacableActor.h"
class Chest : public PlacableActor
{
public:
	Chest(int x, int y, int value);

	int GetValue() const { return m_value; }

	virtual ActorType GetType() override { return ActorType::Chest; }
	virtual void Draw() override;

private:
	int m_value;
};