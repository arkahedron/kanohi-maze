#include "Goal.h"
#include <iostream>

Goal::Goal(int x, int y)
	: PlacableActor(x, y)
{

}

void Goal::Draw()
{
	std::cout << (char)234;
}