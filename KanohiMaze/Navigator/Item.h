#pragma once
#include <string>
#include <iostream>
#include "Randomizer.h"

enum class ItemType { CONSUMABLE, RESOURCE, EQUIPMENT };
enum class Element { NONE = 0, ICE, WATER, AIR, FIRE, STONE, EARTH  };
enum class Rarity { SCRAP, USEFUL, GREAT, EPIC, ANCIENT };

class Item
{

public:

	Item(std::string name, ItemType type, Rarity rarity = Rarity::SCRAP, Element element = Element::NONE);
	~Item();
	void RollElement();
	void RollRarity(int range = 2);
	void Draw();
	void Print();

	std::string m_name;
	Rarity GetRarity() { return m_rarity; };
	Element GetElement() { return m_element; };

protected:
	ItemType m_type;
	Rarity m_rarity;
	Element m_element;
	Randomizer m_randomizer;
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