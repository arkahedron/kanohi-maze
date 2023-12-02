#include "PlacableActor.h"


PlacableActor::PlacableActor(int x, int y, AColor color)
	: m_pPosition( new Point(x,y))
	, m_IsActive(true)
	, m_color(color)
	, m_symbol(ASymbol::Space)
{

}
PlacableActor::~PlacableActor()
{
	//delete m_pPosition;
	//m_pPosition = nullptr;
}

int PlacableActor::GetXPosition()
{
	return m_pPosition->x;
}
int PlacableActor::GetYPosition()
{
	return m_pPosition->y;
}
int* PlacableActor::GetXPositionPointer()
{
	return &(m_pPosition->x);
}
int* PlacableActor::GetYPositionPointer()
{
	return &(m_pPosition->y);
}
void PlacableActor::SetPosition(int x, int y)
{
	m_pPosition->x = x;
	m_pPosition->y = y;
}
void PlacableActor::Place(int x, int y)
{
	m_pPosition->x = x;
	m_pPosition->y = y;
	m_IsActive = true;
}