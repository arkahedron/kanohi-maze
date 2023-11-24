#pragma once
#include <string>
#include "Visuals.h"

class Input
{
	Visuals m_visuals;
	char m_mainInput;

public:
	Input();
	~Input();

	bool BinaryChoice(std::string);
};