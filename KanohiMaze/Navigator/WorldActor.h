#pragma once
#include "Point.h"
#include "Visuals.h"
#include "Input.h"

enum class ActorType
{
	Door,
	Enemy,
	Goal,
	Key,
	Chest,
	Ore,
	Player,
};

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
	ASymbol GetSymbol() { return m_symbol; }

	bool IsActive() { return m_IsActive; }
	void Place(int x, int y);
	void Remove();

	virtual ActorType GetType() { return ActorType::Player; };
	virtual bool Interact();
	
	virtual void Draw();
	virtual void Update();

	//void SetPlayerRef(Player* pRef) { m_pPlayerRef = pRef; };
protected:
	Visuals m_visuals;
	Input m_input;
	Point* m_pPosition;
	//Player* m_pPlayerRef;
	
	
	bool m_IsSolid;
	bool m_IsActive;
	AColor m_color;
	ASymbol m_symbol;

};

