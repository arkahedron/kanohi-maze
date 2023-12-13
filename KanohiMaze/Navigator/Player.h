#pragma once
#include <vector>
#include "WorldActor.h"
#include "Point.h"
#include "Visuals.h"
#include "Input.h"

enum class Direction 
{ 
	none = 0,
	Up = 65, 
	Down = 86, 
	Left = 60, 
	Right = 62, 
};

class Level;
class WorldActor;
class Item;
class Key;

class Player
{
	Input m_input;
	Point m_position;
protected:
	Visuals m_visuals;
	Player();
	~Player();
	static Player* instance;
public:
	static Player* GetInstance();

	WorldActor m_WorldActor;
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

	void ListInventory();
	void OpenMenu();

	bool HandleMovement();
	void Interact();

	int GetLives() { return lives; }
	void DecrementLives() { lives--; }

};

