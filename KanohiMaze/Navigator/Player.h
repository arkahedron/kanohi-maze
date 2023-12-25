#pragma once
#include <vector>

#include "WorldActor.h"
#include "Point.h"
#include "Visuals.h"
#include "Input.h"

using namespace std;

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
class SaveManager;

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
	void DestroyPlayer() { delete instance; instance = nullptr; }

	WorldActor m_WorldActor;

	vector<string> m_itemList;
	vector<vector<Item*>> m_inventory{};

	bool menuIsOpen;
	bool exited;

	Direction playerFacing;
	void SetFacingDirection(Direction pFacing);

	Item* CreateNewItem(string itemType, string rarity = "");
	void PickupItem(Item* pItem);

	bool FindKey(bool spendKey = false);

	void ListInventory();

	void LoadInventory();
	void SaveInventory();
	void ClearInventory();

	void OpenMenu();

	bool HandleMovement();
	void Interact();
};

