#include "Player.h"
#include <iostream>
#include <Windows.h>

using namespace std;

char kPlayerSymbol = 48; //4
constexpr int colorBase = 7;

Player::Player()
	: m_lookDirection(0)
	, exited(false)
{

}
Player::~Player()
{

}


void Player::SetPosition(int x, int y)
{
	m_position.x = x;
	m_position.y = y;
}
void Player::SetLookDirection(int z)
{
	m_lookDirection = z;

	switch (z) 
	{
	case 1:
	{ kPlayerSymbol = 65; /*30*/ break; }
	case 2:
	{ kPlayerSymbol = 86; /*31*/ break; }
	case 3:
	{ kPlayerSymbol = 60; /*17*/ break; }
	case 4:
	{ kPlayerSymbol = 62; /*16*/ break; }
	}
}

void Player::OpenMenu()
{
	exited = m_menu.Open(m_inventory);
}

bool Player::HasKey()
{
	return ((m_inventory.keys > 0) ? true : false);
}
void Player::PickupKey(int amt)
{
	m_inventory.keys += amt;
}
void Player::UseKey()
{
	m_inventory.keys--;
}
void Player::PickupMat(int amt)
{
	m_inventory.mats += amt;
}

void Player::Draw()
{
	m_visuals.ColorText(11);
	cout << kPlayerSymbol;
	m_visuals.ColorText(colorBase);
}