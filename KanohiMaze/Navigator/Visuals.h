#pragma once
#include <string>
#include <Windows.h>
#include <iostream>

enum class AColor
{
	DarkBlue = 1, /**/
	DarkGreen = 2, /**/
	DarkTeal = 3, /**/
	DarkRed = 4, /**/
	Purple = 5,/*Epic, */
	Orange = 6, /*Stone, chest, container*/
	Regular = 7, /*base*/
	Grey = 8, /*Earth, ores, items*/
	Blue = 9, /*Water, */
	Green = 10, /*Air, */
	Teal = 11, /*Ice, player*/
	Red = 12, /*Fire, */
	Pink = 13, /*Ancient, */
	Yellow = 14, /*Great, keys*/
	White = 15, /*Decent, exit, pathways*/
	SolidGreen = 34, /**/
	SolidRed = 68, /**/
	SolidBlue = 153, /**/
	Inverted = 240, /* inverts text, controls*/
};

enum class ASymbol
{
	Space = 0,
	Door = 35,
	Enemy = 153,
	Key = 162,
	Wall = 219,
	Ore = 232,
	Goal = 234,
	Chest = 239,
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

	void ColorText(AColor color = AColor::Regular);
	void ResetTextColor();

	void PrintOutActor(char actor, AColor color = AColor::Regular);

	void DrawMazeControls();

	void DrawTop();
	void DrawBottom();
	void DrawLeft(int y);
	void DrawRight(int y);
private:
	Level* levelRef;
};