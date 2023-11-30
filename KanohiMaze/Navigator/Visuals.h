#pragma once
#include <string>
#include <Windows.h>
#include <iostream>

enum class AColor
{
	DarkGreen = 2,
	DarkTeal = 3,
	DarkRed = 4,
	Purple = 5,
	Orange = 6,
	Regular = 7,
	Grey = 8,
	Blue = 9,
	Green = 10,
	Teal = 11,
	Red = 12,
	Pink = 13,
	Yellow = 14,
	White = 15,
	SolidGreen = 34,
	SolidRed = 68,
	SolidBlue = 153,
};

class Level;

class Visuals
{

public:
	Visuals();
	~Visuals();

	void SetLevelRef(Level* lvlRef) { levelRef = lvlRef; };

	COORD FindCursorPos();
	void ResetCursor(COORD cPos = {0,0});
	void WipeLastLines(int x);

	void DrawAtSpace(int x, int y, char thing);

	void ColorText(int color);

	void PrintOutActor(char actor, AColor color = AColor::Regular);

	void DrawControls();

	void DrawTop();
	void DrawBottom();
	void DrawLeft(int y);
	void DrawRight(int y);
private:
	Level* levelRef;
};