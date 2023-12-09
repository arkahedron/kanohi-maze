#pragma once
#include "WorldActor.h"

class Player;
class Door : public WorldActor
{
public:
	Door(int x = 0, int y = 0);
	virtual ActorType GetType() override { return ActorType::Door; }

	bool IsOpen() { return m_isOpen; }
	void Open() { m_isOpen = true; }

	void Update() override;
	void Unlock();

private:
	bool m_isOpen;
};