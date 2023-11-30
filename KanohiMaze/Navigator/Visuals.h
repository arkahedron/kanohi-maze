#pragma once
#include <string>
#include <Windows.h>
#include <iostream>

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

	void DrawControls();

	void DrawTop();
	void DrawBottom();
	void DrawLeft(int y);
	void DrawRight(int y);
private:
	Level* levelRef;
};