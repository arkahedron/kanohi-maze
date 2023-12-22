#include "Wall.h"

Wall::Wall(int x, int y)
	: WorldActor(x, y, AColor::Grey, ASymbol::Wall)
{
	m_IsSolid = true;
}
