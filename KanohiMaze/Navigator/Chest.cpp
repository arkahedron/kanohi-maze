#include "Chest.h"

Chest::Chest(int x, int y)
	: WorldActor(x, y, AColor::Orange, ASymbol::Chest)
{
	m_IsSolid = false;
}