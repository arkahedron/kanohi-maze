#include "WorldActor.h"



WorldActor::WorldActor(int x, int y, AColor color, ASymbol symbol)
	: /*m_pPlayerRef(nullptr) ,*/
	 m_pPosition(new Point(x, y))
	, m_IsSolid(false)
	, m_IsActive(true)
	, m_color(color)
	, m_symbol(symbol)
{

}
WorldActor::~WorldActor()
{
	//delete m_pPosition;
	//m_pPosition = nullptr;
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
	m_pPosition->x = -1;
	m_pPosition->y = -1;
}

void WorldActor::Draw()
{
	if (m_IsActive)
	{
		m_visuals.ColorText(m_color);
		m_visuals.DrawAtSpace(m_pPosition->x, m_pPosition->y, (char)m_symbol);
		m_visuals.ResetTextColor();
	}
}

void WorldActor::Update()
{
	Draw();
}