#pragma once
#include "Point.h"

constexpr int colorGreen = 10;
constexpr int colorRed = 12;
constexpr int colorBlue = 9;
constexpr int colorRegular = 7;

class PlacableActor
{
public:
	PlacableActor(int x, int y, int color = colorRegular);
	virtual ~PlacableActor();

	bool exited;

	int GetXPosition();
	int GetYPosition();
	int* GetXPositionPointer();
	int* GetYPositionPointer();
	void SetPosition(int x, int y);

	int GetColor() { return m_color; }

	void Remove() { m_IsActive = false; }
	bool IsActive() { return m_IsActive; }
	void Place(int x, int y);

	virtual void Draw() = 0;
	virtual void Update()
	{

	}

protected:
	Point* m_pPosition;
	
	bool m_IsActive;
	int m_color;
};

