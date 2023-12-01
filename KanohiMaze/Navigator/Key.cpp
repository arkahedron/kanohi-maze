#include "Key.h"
#include <iostream>
#include <Windows.h>

void Key::Draw()
{
	m_visuals.ColorText(AColor::Yellow);
	std::cout << "~";
	m_visuals.ResetTextColor();
}