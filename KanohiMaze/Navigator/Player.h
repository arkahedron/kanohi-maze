#pragma once
#include "PlacableActor.h"
#include "Point.h"
#include "Visuals.h"
#include "Input.h"

class Key;

class Player : public PlacableActor
{
	Input m_input;
	Point m_position;
	Visuals m_visuals;

public:
	Player();
	~Player();

	int lookDirection;
	bool menuIsOpen;
	bool exited;
	int keys;
	int mats;
	int lives;

	void SetLookDirection(int z);

	void OpenMenu();

	bool HasKey();
	void PickupKey(int amt);
	void UseKey();

	void PickupMat(int amt);

	int GetLives() { return lives; }
	void DecrementLives() { lives--; }

	char GoodDraw();

	virtual ActorType GetType() override { return ActorType::Player; }
	virtual void  Draw() override;
};