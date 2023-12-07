#include "Door.h"
#include "Player.h"

Door::Door(int x, int y)
	: PlacableActor(x, y, AColor::White, ASymbol::Door)
	, m_isOpen(false)
	, m_openColor(AColor::Green)
{
	m_IsSolid = true;
}


//void Door::Update()
//{
//	bool playerHasKey = m_pPlayerRef->HasKey();
//	if (m_pPlayerRef)
//	{
//		if (playerHasKey)
//		{
//			m_color = m_openColor;
//		}
//		else { m_color = m_color; }
//	}
//	Draw();
//}


void Door::Unlock()
{
	m_IsSolid = false;
	
	Update();

}