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

class WorldActor;
class Item;
class Key;

class Player
{
	
	Input m_input;
	Point m_position;
	Visuals m_visuals;
protected:
	static Player* instance;
	Player();
	~Player();
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

	int GetLives() { return lives; }
	void DecrementLives() { lives--; }




};

