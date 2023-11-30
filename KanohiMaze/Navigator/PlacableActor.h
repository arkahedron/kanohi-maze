#pragma once
#include "Point.h"
#include "Visuals.h"

enum class ActorType
{
	Door,
	Enemy,
	Goal,
	Key,
	Chest,
	Player,
};

class PlacableActor
{
public:
	PlacableActor(int x, int y, AColor color = AColor::Regular);
	virtual ~PlacableActor();

	int GetXPosition();
	int GetYPosition();
	int* GetXPositionPointer();
	int* GetYPositionPointer();
	void SetPosition(int x, int y);

	AColor GetColor() { return m_color; }

	void Remove() { m_IsActive = false; }
	bool IsActive() { return m_IsActive; }
	void Place(int x, int y);

	virtual ActorType GetType() = 0;
	virtual void Draw() = 0;
	virtual void Update()
	{

	}

protected:
	Point* m_pPosition;
	
	bool m_IsActive;
	AColor m_color;
};

