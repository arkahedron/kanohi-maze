#include <iostream>
#include <Windows.h>

#include "Key.h"

void Key::Draw()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, (int)AColor::Yellow);

	std::cout << "~";
	SetConsoleTextAttribute(console, (int)AColor::Regular);
}