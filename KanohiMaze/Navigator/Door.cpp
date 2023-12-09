#include "Door.h"
#include "Player.h"

Door::Door(int x, int y)
	: WorldActor(x, y, AColor::Red, ASymbol::Door)
	, m_isOpen(false)
{
	m_IsSolid = true;
}


void Door::Update()
{
	bool playerHasKey = Player::GetInstance()->HasKey();
	if (playerHasKey)
	{
		m_color = AColor::Green;
	}
	else { m_color = AColor::Red; }
	Draw();
}


void Door::Unlock()
{
	m_IsSolid = false;
	
	Update();

}