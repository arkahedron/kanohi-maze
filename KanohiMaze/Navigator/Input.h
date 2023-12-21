#pragma once
#include <string>
#include <vector>
#include "Visuals.h"

using namespace std;

class Input
{
	Visuals m_visuals;
	char m_mainInput;

public:
	Input();
	~Input();

	bool BinaryChoice(string);

	string VerticalMenu(vector<string> stringList, COORD buffer, bool enclose = false);

};