#pragma once
#include "Point.h"
#include "Visuals.h"
#include "Inventory.h"
#include "Menu.h"

class Player
{
	Point m_position;
	Inventory m_inventory;
	Visuals m_visuals;
	Menu m_menu;
	int m_lookDirection;

public:
	Player();
	~Player();

	bool exited;

	int GetXPosition() { return m_position.x; };
	int GetYPosition() { return m_position.y; };

	int* GetXPositionPointer() { return &(m_position.x); }
	int* GetYPositionPointer() { return &(m_position.y); }

	int GetLookDirection() { return m_lookDirection; };
	int* GetLookDirectionPointer() { return &(m_lookDirection); }

	void SetPosition(int x, int y);
	void SetLookDirection(int z);

	Inventory GetInvRef(){ return m_inventory; }

	void OpenMenu();

	bool HasKey();
	void PickupKey(int amt);
	void UseKey();
	void PickupMat(int amt);

	void Draw();
};