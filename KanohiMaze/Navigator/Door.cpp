#include "Door.h"
#include <iostream>
#include <Windows.h>

Door::Door(int x, int y, AColor color, AColor closedColor)
	: PlacableActor(x, y, color)
	, m_isOpen(false)
	, m_closedColor(closedColor)
{

}

void Door::Draw()
{
	if (m_isOpen)
	{ m_visuals.ColorText(m_color); }
	else
	{ m_visuals.ColorText(m_closedColor); }
	std::cout << "#";
	m_visuals.ResetTextColor();
}