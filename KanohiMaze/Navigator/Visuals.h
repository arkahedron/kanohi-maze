#pragma once
//#include "Level.h"
#include <string>

class Visuals
{
public:
	Visuals();
	~Visuals();

	//void SetLevelRef(Level lvlRef) { Level levelRef = lvlRef; };

	void ColorText(int color);

	void SubText(std::string txt);

	void DrawControls();

	void DrawTop(int width);
	void DrawBottom(int width);
	void DrawLeft(int y);
	void DrawRight(int y, int width);
};