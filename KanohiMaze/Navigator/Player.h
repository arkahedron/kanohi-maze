#pragma once
#include <vector>
#include "PlacableActor.h"
#include "Point.h"
#include "Visuals.h"
#include "Input.h"

enum class Direction 
{ 
	none = 0,
	up = 65, 
	down = 86, 
	left = 60, 
	right = 62, 
};

class Item;
class Key;

class Player : public PlacableActor
{
	Input m_input;
	Point m_position;
	Visuals m_visuals;

public:
	Player();
	~Player();

	std::vector<Item*> m_pItems;

	bool menuIsOpen;
	bool exited;
	int keys;
	int mats;
	int lives;
	Direction playerFacing;

	void SetFacingDirection(Direction pFacing);

	bool HasKey();
	void PickupKey(int amt);
	void UseKey();

	void PickupMat(int amt);

	char GoodDraw();

	void OpenMenu();

	int GetLives() { return lives; }
	void DecrementLives() { lives--; }

	virtual ActorType GetType() override { return ActorType::Player; }
	virtual void  Draw() override;
};