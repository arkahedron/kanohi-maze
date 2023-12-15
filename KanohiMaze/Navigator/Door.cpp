#include "Door.h"
#include "Player.h"
#include <iostream>
#include <vector>

#include "Item.h"

Door::Door(int x, int y)
	: WorldActor(x, y, AColor::Red, ASymbol::Door)
	, m_isOpen(false)
{
	m_IsSolid = true;
}


void Door::Update()
{
	bool pHasKey = Player::GetInstance()->FindKey();
	if (pHasKey)
	{ 
		m_color = AColor::Green;
	}
	else { m_color = AColor::Red; }
	Draw();
}

bool Door::Interact()
{
	bool pHasKey = Player::GetInstance()->FindKey();
	if (pHasKey)
	{
		if (m_input.BinaryChoice("USE KEY ON DOOR?"))
		{
			Player::GetInstance()->FindKey(true);
			//Player::GetInstance()->m_pItems.erase(std::remove(Player::GetInstance()->m_pItems.begin(), Player::GetInstance()->m_pItems.end(), pHasKey), Player::GetInstance()->m_pItems.end());
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
