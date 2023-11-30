#pragma once
#include <string>
#include <iostream>

class Item
{

public:
	std::string m_name;
	int m_value;
	virtual void Draw() = 0;
};

class Key : public Item
{
public:
	enum Type {};
	void Draw() override
	{
		std::cout << '~';
	}
};

class Antitoxin : public Item
{
public:
	enum Type {};
	void Draw() override
	{
		std::cout << '~';
	}
};

class Ore : public Item
{
public:
	void Draw() override
	{
		std::cout << '8';
	}
};

class Protosteel : public Item
{
public:
	void Draw() override
	{
		std::cout << '$';
	}
};

class KanokaDisc : public Item
{
public:
	void Draw() override
	{
		std::cout << '0';
	}
};

class MaskMold : public Item
{
public:
	void Draw() override
	{
		std::cout << 'U';
	}
};

class KanohiMask : public Item
{
public:
	void Draw() override
	{
		std::cout << 'M';
	}
};