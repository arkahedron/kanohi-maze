#pragma once
#include <string>
#include "Visuals.h"
#include "Input.h"
#include "Inventory.h"

class Menu
{
	Visuals m_visuals;
	Input m_input;
	Inventory m_inventory;
	//bool m_didExit;
	bool m_isOpen;

public:
	Menu();
	~Menu();


	bool Open(Inventory inv);
	//bool CheckExited() { return m_didExit; };
};