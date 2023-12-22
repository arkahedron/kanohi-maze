#include "Visuals.h"
#include "Level.h"
#include <string>
#include <windows.h>
#include <iostream>

using namespace std;

static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

constexpr unsigned char kHorizontalBorder = 205;
constexpr unsigned char kTopLeftBorder = 201;
constexpr unsigned char kTopRightBorder = 187;
constexpr unsigned char kBottomLeftBorder = 200;
constexpr unsigned char kBottomRightBorder = 188;
constexpr unsigned char kVerticalBorder = 186;
constexpr unsigned char kHole = 206;
constexpr unsigned char WAL = (char)219;
constexpr unsigned char GOL = (char)234;

Visuals::Visuals()
	: levelRef(nullptr)
	, edgeWidth(15)
	, edgeHeight(6)
{

}
Visuals::~Visuals()
{

}

COORD Visuals::FindCursorPos()
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(console, &cbsi))
	{
		return cbsi.dwCursorPosition;
	}
	else{ COORD invalid = {0,0}; return invalid;}
}
void Visuals::ResetCursor(COORD cPos, bool toIdle)
{
	cout.flush();
	COORD cursorPos = cPos;
	if (toIdle)
	{ cursorPos.Y += edgeHeight + 4; }
	SetConsoleCursorPosition(console, cursorPos);
	cout.flush();
}
void Visuals::WipeLastLines(int x)
{
	COORD replace = FindCursorPos();
	for (int y = 0; y < x; y++)
	{
		printf("\33[2K");
		replace.Y -= 1;
		ResetCursor(replace);
	}
	printf("\33[2K");
}

void Visuals::PrintOutActor(char actor, AColor color)
{ 
	ColorText(color);
	cout << actor;
	ResetTextColor(); 
}
void Visuals::DrawAtSpace(int x, int y, char thing, COORD bufferSpace)
{
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	/*Temp harsh-defined buffer for edges and border to align map draw*/
	coord.X += 4; /*3*/
	coord.Y += 2; /*2*/
	coord.X += bufferSpace.X;
	coord.Y += bufferSpace.Y;
	SetConsoleCursorPosition(console, coord);
	cout << thing;
	cout.flush();
	COORD idle = { 0,0 };
	idle.Y += edgeHeight + 4;
	SetConsoleCursorPosition(console, idle);
}

void Visuals::ColorText(AColor color)
{ SetConsoleTextAttribute(console, (int)color); }
void Visuals::ResetTextColor() 
{ SetConsoleTextAttribute(console, (int)AColor::Regular); }

void Visuals::DrawTop()
{
	ResetTextColor();
	cout << "\n" << "  " ; /* 1 down 1 right buffer*/
	cout << kTopLeftBorder;
	for (int i = 0; i < levelRef->m_width; i++)
	{
		if (levelRef->m_holeY == 0 && levelRef->m_holeX == i) 
		{ cout << kHole; }
		else { cout << kHorizontalBorder; }
	}
	cout << kTopRightBorder << endl;
}
void Visuals::DrawBottom()
{
	ResetTextColor();
	cout << "  "; /* 1 right buffer*/
	cout << kBottomLeftBorder;
	for (int i = 0; i < levelRef->m_width; i++)
	{
		if (levelRef->m_holeY == levelRef->m_height - 1 && levelRef->m_holeX == i) 
		{ cout << kHole; }
		else { cout << kHorizontalBorder; }
	} 
	cout << kBottomRightBorder;
	cout << endl;
}
void Visuals::DrawLeft(int y)
{
	ResetTextColor();
	cout << "  "; /* 1 right buffer*/
	if (levelRef->m_holeX == 0 && levelRef->m_holeY == y) 
	{ cout << kHole; }
	else { cout << kVerticalBorder; }
	
}
void Visuals::DrawRight(int y)
{
	ResetTextColor();
	if (levelRef->m_holeX == levelRef->m_width - 1 && levelRef->m_holeY == y) 
	{ cout << kHole; }
	else { cout << kVerticalBorder; }
	cout << endl;
	
}

void Visuals::DrawFrame()
{
	ColorText(AColor::White);
	COORD frameAlign = { (-2),(0) };
	//cout << WAL;
	for (int t = 0; t < edgeWidth+2; t++) { DrawAtSpace(t,-1, WAL, frameAlign); }
	//cout << WAL;

	for (int y = 0; y < edgeHeight+2; ++y)
	{
		DrawAtSpace(0, y, WAL, frameAlign);
		DrawAtSpace(1, y, WAL, frameAlign);

		DrawAtSpace(edgeWidth, y, WAL, frameAlign);
		DrawAtSpace(edgeWidth+1, y, WAL, frameAlign);
	}

	//cout << WAL;
	for (int b = 0; b < edgeWidth+2; b++) { DrawAtSpace(b, edgeHeight+1, WAL, frameAlign); }
	//cout << WAL;
	ResetTextColor();
}

void Visuals::DrawMazeControls()
{

	COORD menuCoord = { (edgeWidth+4), (6) };

	SetConsoleCursorPosition(console, menuCoord);
	ColorText(AColor::Inverted);
	cout << " Move: WASD ";
	menuCoord.Y += 1;
	//SetConsoleCursorPosition(console, menuCoord);
	//cout << "          ";
	//menuCoord.Y += 1;
	//SetConsoleCursorPosition(console, menuCoord);
	//ColorText(AColor::Inverted);
	//cout << "Look: SHFT+WASD" <<
	//endl << "               ";
	//menuCoord.Y += 2;
	SetConsoleCursorPosition(console, menuCoord);
	cout << " Act: E     ";
	menuCoord.Y += 1;
	//SetConsoleCursorPosition(console, menuCoord);
	//cout << "          ";
	//menuCoord.Y += 1;
	SetConsoleCursorPosition(console, menuCoord);
	cout << " Menu: TAB  ";
	menuCoord.Y += 1;
	//SetConsoleCursorPosition(console, menuCoord);
	//cout << "          ";
	//menuCoord.Y += 1;

	if (true) {
		ColorText(AColor::White);

		int width = edgeWidth;
		width = 12;
		int height = edgeHeight;
		COORD frameAlign = { (edgeWidth),(0) };
		//cout << WAL;
		for (int t = 0; t < width + 2; t++) { DrawAtSpace(t, -1, WAL, frameAlign); }
		//cout << WAL;

		for (int y = 0; y < height + 2; ++y)
		{
			//DrawAtSpace(0, y, WAL, frameAlign);
			//DrawAtSpace(1, y, WAL, frameAlign);

			DrawAtSpace(width, y, WAL, frameAlign);
			DrawAtSpace(width + 1, y, WAL, frameAlign);
		}

		//cout << WAL;
		for (int b = 0; b < width + 2; b++) { DrawAtSpace(b, height + 1, WAL, frameAlign); }
		//cout << WAL;
	}
	ResetTextColor();

}