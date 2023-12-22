#pragma once
#include "Point.h"
#include "Visuals.h"
#include "Input.h"

enum class ActorType
{
	Door,
	Wall,
	Enemy,
	Goal,
	Key,
	Chest,
	Ore,
	Player,
};

class Item;
class Player;
class WorldActor
{
public:
	WorldActor(int x, int y, AColor color = AColor::Regular, ASymbol symbol = ASymbol::Space);
	virtual ~WorldActor();

	int GetXPosition();
	int GetYPosition();
	int* GetXPositionPointer();
	int* GetYPositionPointer();
	void SetPosition(int x, int y);

	bool GetSolidity() { return m_IsSolid; }
	AColor GetColor() { return m_color; }
	void SetSymbol(char symbol) { m_symbol = static_cast<ASymbol>(symbol); }
	char GetSymbol() { return (char)static_cast<int>(m_symbol); }

	bool IsActive() { return m_IsActive; }
	void Place(int x, int y);
	void Remove();

	virtual ActorType GetType() { return ActorType::Player; };
	virtual bool Interact();
	virtual bool PlayerOverlap();
	
	virtual void Draw();
	virtual void Update();
	bool m_IsActive;

	//void SetPlayerRef(Player* pRef) { m_pPlayerRef = pRef; };
	COORD m_drawPos;
	COORD m_drawMod;
	void ModDrawPos(COORD posOffset);
protected:
	Visuals m_visuals;
	Input m_input;
	Point* m_pPosition;
	
	bool m_IsSolid;

	AColor m_color;
	ASymbol m_symbol;

};

