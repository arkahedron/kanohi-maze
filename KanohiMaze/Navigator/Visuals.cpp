#include "Visuals.h"
#include "Level.h"
#include <string>
#include <windows.h>
#include <iostream>

using namespace std;

static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

constexpr char kHorizontalBorder = 205;
constexpr char kTopLeftBorder = 201;
constexpr char kTopRightBorder = 187;
constexpr char kBottomLeftBorder = 200;
constexpr char kBottomRightBorder = 188;
constexpr char kVerticalBorder = 186;
constexpr char kHole = 206;

Visuals::Visuals()
	: levelRef(nullptr)
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
void Visuals::ResetCursor(COORD cPos)
{
	COORD cursorPos = cPos;
	cout.flush();
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
void Visuals::DrawAtSpace(int x, int y, char thing)
{
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	/*Temp harsh-defined buffer for edges and border to align map draw*/
	coord.X += 3; 
	coord.Y += 2;
	SetConsoleCursorPosition(console, coord);
	cout << thing;
	cout.flush();
	COORD idle = { 0,0 };
	if (levelRef) { idle.Y += (levelRef->m_height + 5); }
	SetConsoleCursorPosition(console, idle);
}

void Visuals::ColorText(AColor color)
{ SetConsoleTextAttribute(console, (int)color); }
void Visuals::ResetTextColor() 
{ SetConsoleTextAttribute(console, (int)AColor::Regular); }

void Visuals::DrawMazeControls()
{
	cout << "  ";
	ColorText(AColor::Inverted);
	cout << " >Move: WASD" << " |";
	cout << " >Look: SHFT+WASD " << endl;
	ResetTextColor();
	cout << "  ";
	ColorText(AColor::Inverted);
	cout << " >Act: E" << "     |";
	cout << " >Menu: TAB       " << endl;
	ResetTextColor();
}


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
