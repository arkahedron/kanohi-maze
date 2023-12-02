#include "Item.h"
#include <iostream>

using namespace std;

static int a_scraps, a_usefuls, a_greats, a_epics, a_ancients; /*temp loot roll data aggregator*/

Item::Item(string name, ItemType type, Rarity rarity, Element element)
	: m_type(type)
	, m_rarity(rarity)
	, m_element(element)
	, m_name(name)
{
	/*either implement trait rolling into constructor or reroll after creation*/
}
Item::~Item() {}


void Item::RollElement()
{
	int entries = (int)Element::EARTH;
	int ePick = m_randomizer.Generate(0, entries);
	m_element = static_cast<Element>(ePick);
}

void Item::RollRarity(int range)
{
	int maxPick;
	switch (range)
	{
	case 1:
		maxPick = (int)Rarity::DECENT;
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



void Item::Draw() {

}

void Item::Print()
{
	m_visuals.ResetTextColor();
	switch (m_rarity)
	{
	case Rarity::NONE:
		break;
	case Rarity::SCRAP:
		a_scraps++;
		cout << "Scrap ";
		break;
	case Rarity::DECENT:
		m_visuals.ColorText(AColor::White);
		a_usefuls++;
		cout << "Decent ";
		break;
	case Rarity::GREAT:
		m_visuals.ColorText(AColor::Yellow);
		a_greats++;
		cout << "Great ";
		break;
	case Rarity::EPIC:
		m_visuals.ColorText(AColor::Purple);
		a_epics++;
		cout << "Epic ";
		break;
	case Rarity::ANCIENT:
		m_visuals.ColorText(AColor::Pink);
		a_ancients++;
		cout << " Ancient ";
		break;
	default:
		break;
	}
	m_visuals.ResetTextColor();
	cout << m_name;
	if(m_element != Element::NONE)
	{
		cout << " of ";
		switch (m_element)
		{
		case Element::NONE:
			break;
		case Element::ICE:
			m_visuals.ColorText(AColor::Teal);
			cout << "Ice";
			break;
		case Element::WATER:
			m_visuals.ColorText(AColor::Blue);
			cout << "Water";
			break;
		case Element::AIR:
			m_visuals.ColorText(AColor::Green);
			::cout << "Air";
			break;
		case Element::FIRE:
			m_visuals.ColorText(AColor::Red);
			cout << "Fire";
			break;
		case Element::STONE:
			m_visuals.ColorText(AColor::Orange);
			cout << "Stone";
			break;
		case Element::EARTH:
			m_visuals.ColorText(AColor::Grey);
			cout << "Earth";
			break;
		default:
			m_visuals.ResetTextColor();
			break;
		}
	}
	m_visuals.ResetTextColor();
	//cout << m_type;
}

void Item::ListLootData()
{
	cout << endl;
	cout << " Scraps: " << a_scraps << endl;
	cout << " Usefuls: " << a_usefuls << endl;
	cout << " Greats: " << a_greats << endl;
	cout << " Epics: " << a_epics << endl;
	cout << " Ancients: " << a_ancients << endl;
}
