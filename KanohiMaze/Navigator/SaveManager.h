#pragma once
#include <string>

#include "Visuals.h"

using namespace std;

class Player;

class SaveManager
{
	Player* m_player;
	Visuals m_visuals;
protected:
	SaveManager();
	~SaveManager();
	static SaveManager* instance;
public:
	static SaveManager* GetInstance();
	void DestroySaveManger() { delete instance; instance = nullptr; }

	string activeSaveFile;

	string CreateSaveFile();
	string FindSaveFiles();
	bool LoadSaveFile(string saveToLoad);
	void WriteToSaveFile();

};

