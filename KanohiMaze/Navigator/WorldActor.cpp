#include "WorldActor.h"

#include <iostream>
#include <string>

#include "Level.h"

class Level;

WorldActor::WorldActor(int x, int y, AColor color, ASymbol symbol)
	: m_pPosition(new Point(x, y))
	, m_IsSolid(false)
	, m_IsActive(true)
	, m_color(color)
	, m_symbol(symbol)
{

}
WorldActor::~WorldActor()
{
	//std::cout << "...deleting " << (char)m_symbol;
	delete m_pPosition;
	m_pPosition = nullptr;
}


int WorldActor::GetXPosition()
{
		return m_pPosition->x;
}
int WorldActor::GetYPosition()
{
		return m_pPosition->y;
}
int* WorldActor::GetXPositionPointer()
{
	return &(m_pPosition->x);
}
int* WorldActor::GetYPositionPointer()
{
	return &(m_pPosition->y);
}
void WorldActor::SetPosition(int x, int y)
{
	m_pPosition->x = x;
	m_pPosition->y = y;
}
void WorldActor::Place(int x, int y)
{
	m_pPosition->x = x;
	m_pPosition->y = y;
	m_IsActive = true;
}

void WorldActor::Remove()
{
	m_IsActive = false;
	m_IsSolid = false;
	Level::GetInstance()->ClearSpace(m_pPosition->x, m_pPosition->y, this);
	delete this;

}

bool WorldActor::Interact()
{
	Update();
	return false;
}

void WorldActor::Draw()
{
	if (m_IsActive)
	{
		m_visuals.ColorText(m_color);
		m_visuals.DrawAtSpace(m_pPosition->x, m_pPosition->y, (char)m_symbol);
		m_visuals.ResetTextColor();
	}
	else 
	{
		m_visuals.DrawAtSpace(m_pPosition->x, m_pPosition->y, ' ');
	}
}

void WorldActor::Update()
{
	Draw();
}