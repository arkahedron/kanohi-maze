#pragma once
#include "Point.h"
#include "Visuals.h"
#include "Input.h"

class Player
{
	Input m_input;
	Point m_position;
	Visuals m_visuals;

public:
	Player();
	~Player();

	int lookDirection;
	bool menuIsOpen;
	int keys;
	int mats;
	bool exited;

	int GetXPosition() { return m_position.x; };
	int GetYPosition() { return m_position.y; };
	int* GetXPositionPointer() { return &(m_position.x); }
	int* GetYPositionPointer() { return &(m_position.y); }
	void SetPosition(int x, int y);

	void SetLookDirection(int z);

	void OpenMenu();

	bool HasKey();
	void PickupKey(int amt);
	void UseKey();
	void PickupMat(int amt);

	char Draw();
};