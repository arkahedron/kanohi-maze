#pragma once
#include <string>

class Level;

class Visuals
{

public:
	Visuals();
	~Visuals();

	void SetLevelRef(Level* lvlRef) { levelRef = lvlRef; };

	void ColorText(int color);

	void SubText(std::string txt);

	void DrawControls();

	void DrawTop();
	void DrawBottom();
	void DrawLeft(int y);
	void DrawRight(int y);
private:
	Level* levelRef;
};