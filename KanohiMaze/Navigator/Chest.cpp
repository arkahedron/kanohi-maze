#include "Chest.h"

Chest::Chest(int x, int y)
	: PlacableActor(x, y, AColor::Orange, ASymbol::Chest)
{
	m_IsSolid = true;
}