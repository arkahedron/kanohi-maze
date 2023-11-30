#include "Item.h"
#include <iostream>

using namespace std;

static int a_scraps;
static int a_usefuls;
static int a_greats;
static int a_epics;
static int a_ancients;

Item::Item(std::string name, ItemType type, Rarity rarity, Element element)
	: m_type(type)
	, m_rarity(rarity)
	, m_element(element)
	, m_name(name)
{

}
Item::~Item() {}


void Item::RollElement()
{
	int entries = (int)Element::EARTH;
	int ePick = m_randomizer.Generate(1, entries);
	m_element = static_cast<Element>(ePick);
}

void Item::RollRarity(int range)
{
	int maxPick;
	switch (range)
	{
	case 1:
		maxPick = (int)Rarity::USEFUL;
		break;
	case 2:
		maxPick = (int)Rarity::GREAT;
		break;
	case 3: 
		maxPick = (int)Rarity::EPIC;
		break;
	default:
		maxPick = (int)Rarity::EPIC;
		break;
	}
	
	int ePick = m_randomizer.Generate(0, maxPick);
	ePick += m_randomizer.Modulate();
	ePick = abs(ePick);
	m_rarity = static_cast<Rarity>(ePick);
}

void Item::Draw()
{
	cout << endl;
	cout << " Scraps: " << a_scraps << endl;
	cout << " Usefuls: " << a_usefuls << endl;
	cout << " Greats: " << a_greats << endl;
	cout << " Epics: " << a_epics << endl;
	cout << " Ancients: " << a_ancients << endl;
}

void Item::Print()
{
	switch (m_rarity)
	{
	case Rarity::SCRAP:
		a_scraps++;
		std::cout << " Scrap ";
		break;
	case Rarity::USEFUL:
		a_usefuls++;
		std::cout << " Useful ";
		break;
	case Rarity::GREAT:
		a_greats++;
		std::cout << " Great ";
		break;
	case Rarity::EPIC:
		a_epics++;
		std::cout << " Epic ";
		break;
	case Rarity::ANCIENT:
		a_ancients++;
		std::cout << " Ancient ";
		break;
	default:
		break;
	}
	std::cout << m_name;
	switch (m_element)
	{
	case Element::NONE:
		break;
	case Element::ICE:
		std::cout << " of Ice";
		break;
	case Element::WATER:
		std::cout << " of Water";
		break;
	case Element::AIR:
		std::cout << " of Air";
		break;
	case Element::FIRE:
		std::cout << " of Fire";
		break;
	case Element::STONE:
		std::cout << " of Stone";
		break;
	case Element::EARTH:
		std::cout << " of Earth";
		break;
	default:
		break;
	}
	//std::cout << m_type;
}


//
//void Key::Draw()
//{
//	std::cout << '~';
//}