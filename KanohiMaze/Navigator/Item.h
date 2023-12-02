#pragma once
#include <string>
#include <iostream>
#include "Randomizer.h"
#include "Visuals.h"

enum class ItemType { CONSUMABLE, RESOURCE, EQUIPMENT };
enum class Element { NONE = -1, ICE, WATER, AIR, FIRE, STONE, EARTH  };
enum class Rarity { NONE = -1, SCRAP, DECENT, GREAT, EPIC, ANCIENT };

class Item
{

public:
	Visuals m_visuals;

	Item(std::string name, ItemType type, Rarity rarity = Rarity::SCRAP, Element element = Element::NONE);
	~Item();

	void RollElement();
	void RollRarity(int range = 2); /*Rarity ranges are 1-3*/

	void Draw();
	void Print();

	void ListLootData();

	Rarity GetRarity() { return m_rarity; };
	Element GetElement() { return m_element; };
	std::string GetName() { return m_name; };

protected:
	std::string m_name;
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