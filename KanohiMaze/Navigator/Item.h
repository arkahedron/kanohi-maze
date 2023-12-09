#pragma once
#include <string>
#include <iostream>
#include "Randomizer.h"
#include "Visuals.h"

enum class ItemType { CONSUMABLE, RESOURCE, EQUIPMENT };

enum class Rarity { NONE = -1, SCRAP, DECENT, GREAT, EPIC, ANCIENT };
constexpr const char* RarityToStr(Rarity e) noexcept
{
	switch (e)
	{
	case Rarity::NONE: return "";
	case Rarity::SCRAP: return "Scrap";
	case Rarity::DECENT: return "Decent";
	case Rarity::GREAT: return "Great";
	case Rarity::EPIC: return "Epic";
	case Rarity::ANCIENT: return "Ancient";
	default: return "";
	}
}

enum class Element { NONE = -1, ICE, WATER, AIR, FIRE, STONE, EARTH  };
constexpr const char* ElementToStr(Element e) noexcept
{
	switch (e)
	{
	case Element::NONE: return "";
	case Element::ICE: return "Ice";
	case Element::WATER: return "Water";
	case Element::AIR: return "Air";
	case Element::FIRE: return "Fire";
	case Element::STONE: return "Stone";
	case Element::EARTH: return "Earth";
	default: return "";
	}
}


class Item
{

public:
	Visuals m_visuals;
	Randomizer m_randomizer;

	Item(std::string name, ItemType type, Rarity rarity = Rarity::SCRAP, Element element = Element::NONE);
	~Item();

	void RollElement();
	void RollRarity(int range = 2); /*Rarity ranges are 1-3*/

	std::string UpdateFullName();
	void Draw();
	void Print();

	void ListLootData();

	Rarity GetRarity() { return m_rarity; };
	Element GetElement() { return m_element; };
	std::string GetName() { return m_name; };
	std::string GetFullName() { return m_FullName; };

protected:
	std::string m_name;
	std::string m_FullName;
	ItemType m_type;
	Rarity m_rarity;
	Element m_element;

};

//
//
//class Key : public Item
//{
//public:
//	void Draw() override;
//};
//
//class Antitoxin : public Item
//{
//public:
//	void Draw() override
//	{
//		std::cout << '~';
//	}
//};
//
//class Ore : public Item
//{
//public:
//	void Draw() override
//	{
//		std::cout << '8';
//	}
//};
//
//class Protosteel : public Item
//{
//public:
//	void Draw() override
//	{
//		std::cout << '$';
//	}
//};
//
//class KanokaDisc : public Item
//{
//	KanokaDisc(ItemType type, Element element);
//public:
//	void Draw() override
//	{
//		std::cout << '0';
//	}
//};
//
//class MaskMold : public Item
//{
//public:
//	void Item::Draw() override
//	{
//		std::cout << '0';
//	}
//};
//
//class KanohiMask : public Item
//{
//public:
//	void Draw() override
//	{
//		std::cout << 'M';
//	}
//};