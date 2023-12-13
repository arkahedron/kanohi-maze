#include "Door.h"
#include "Player.h"
#include <iostream>

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

bool Door::Interact()
{
	if (bool playerHasKey = Player::GetInstance()->HasKey())
	{
		if (m_input.BinaryChoice("USE KEY ON DOOR?")) {
			Player::GetInstance()->UseKey();
			///PlayDoorOpenEffect();
			std::cout << "\r\33[2K" << " [DOOR OPENED]" << std::endl;
			Remove();
			return true;
			//m_visuals.WipeLastLines(1);
		}
		else { return false; /* m_visuals.WipeLastLines(1); */ }
	}
	else
	{
		///PlayDoorClosedEffect();
		std::cout << "\r\33[2K" << " [DOOR LOCKED]" << std::endl;
		return false;
		//m_visuals.WipeLastLines(1);
	}
	Draw();
}

void Door::Unlock()
{
	m_IsSolid = false;
	
	Update();

}