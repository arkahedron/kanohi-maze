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

//struct Loot {
//	std::string fullName;
//	//Rarity rarity;
//	int quantity;
//};


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

	//std::vector<Loot*> inv_array;

	bool menuIsOpen;
	bool exited;
	int keys;
	int mats;
	Direction playerFacing;

	void SetFacingDirection(Direction pFacing);

	Item* CreatePickedItem();
	void PickupItem(Item* pItem);

	bool FindKey(bool spendKey = false);
	//bool HasKey();
	//void UseKey();

	void ListInventory();
	void OpenMenu();

	bool HandleMovement();
	void Interact();

};

