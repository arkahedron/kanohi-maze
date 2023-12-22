#include "SaveManager.h"

#include <iostream>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <vector>

#include "Player.h"

using namespace std;

constexpr unsigned int kEscapeKey = 27;
constexpr unsigned int kEnterKey = 13;
constexpr unsigned int kUpKey = 72;
constexpr unsigned int kDownKey = 80;

SaveManager::SaveManager()
	: activeSaveFile(" ")
{
	m_player = Player::GetInstance();
}
SaveManager::~SaveManager()
{

}
SaveManager* SaveManager::instance = nullptr;
SaveManager* SaveManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new SaveManager();
	}
	return instance;
}


string SaveManager::CreateSaveFile()
{
	string saveName;

	cout << "\n\r Name your character: ";
	cin >> saveName;
	string characterName = saveName;
	saveName.append(".txt");
	saveName.insert(0, "../Saves/");

	ofstream saveFile;
	saveFile.open(saveName);
	if (!saveFile)
	{
		cout << "\r Opening file failed!" << endl;
	}
	else
	{
		saveFile << characterName << endl;

		/*Write player save data into here, following is placeholder data*/


		if (!saveFile) { cout << "Write failed!" << endl; }
		saveFile.close();
	}
	return saveName;
}

string SaveManager::FindSaveFiles()
{
	//system("cls");

	LPCWSTR lpath = L"../Saves/*.txt";
	vector<wstring> sfListArray;
	wstring saveSelect;
	int saveIndex = 0;

	WIN32_FIND_DATAW ffd;
	HANDLE hFind = FindFirstFileW(lpath, &ffd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				saveIndex++;
				sfListArray.push_back(ffd.cFileName);
			}
		} while (FindNextFileW(hFind, &ffd));
		FindClose(hFind);
	}
	else
	{
		cout << endl << " No valid saves found!" << endl;
		Sleep(900);
		return " ";
	}

	if (saveIndex > 0)
	{
		//bool enclose = false;
		vector<wstring> MenuList = sfListArray;
		unsigned int lMax = MenuList.size();
		int pick = 0;

		while (true)
		{
			cout << "\n  SELECT SAVE: \n";
			//if (enclose) { cout << "  " << string(15, WAL) << endl; }
			for (int i = 0; i < lMax; ++i)
			{
				saveSelect = sfListArray[i];
				string saveListItem(saveSelect.begin(), saveSelect.end());
				saveListItem.erase(saveListItem.find_last_of("."), string::npos);
				//if (enclose) { cout << string(2, WAL); }
				cout << "   ";
				if (i == pick) {
					m_visuals.ColorText(AColor::Inverted);
					cout << ">" << saveListItem << " ";
				}
				else {
					m_visuals.ResetTextColor();
					cout << "" << saveListItem << " ";
				}
				m_visuals.ResetTextColor();
				//if (enclose) { cout << string(2, WAL); }
				cout << endl;
			}
			//if (enclose) { cout << "  " << string(15, WAL) << endl; }

			while (true)
			{
				saveSelect = sfListArray[pick];
				int input = _getch();
				if (input == 'w' || (char)input == kUpKey)
				{
					pick -= 1;
					if (pick == -1) { pick = lMax - 1; }
					break;
				}
				else if (input == 's' || (char)input == kDownKey)
				{
					pick += 1;
					if (pick == lMax) { pick = 0; }
					break;
				}
				else if (input == 'e' || input == kEnterKey)
				{
					string saveInput(saveSelect.begin(), saveSelect.end());
					return saveInput;
					break;
				}
				else if (input == kEscapeKey)
				{
					return " ";
					break;
				}
			}
			Sleep(120);
			m_visuals.WipeLastLines(lMax + 2);
		}
		return " ";
	}
	else { return " "; }
}

bool SaveManager::LoadSaveFile(string saveName)
{
	activeSaveFile = saveName;
	//Get defined text file as level and try to open it
	saveName.insert(0, "../Saves/");
	fstream saveFile;
	saveFile.open(saveName);
	if (saveFile)
	{
		constexpr int tempSize = 25;
		char temp[tempSize];

		saveFile.getline(temp, tempSize, '\n');
		int variedItems = atoi(temp);
		string itemData;
		const int MAX_SIZE = 100;
		int index = 0;
		while (index < MAX_SIZE && saveFile >> itemData)
		{
			index++;
			if (itemData != "")
			{
				m_player->m_itemList.push_back(itemData);
			}
		}

		m_player->LoadInventory();
		m_player->m_itemList.clear();
		saveFile.close();
		return true;
	}
	else {
		//cout << " Invalid save file, please try again!" << endl;
		return false;
	}
}

void SaveManager::WriteToSaveFile()
{
	string saveName = activeSaveFile;
	string characterName = saveName;
	//saveName.append(".txt");
	saveName.insert(0, "../Saves/");

	fstream getSaveFile;
	getSaveFile.open(saveName);
	if (getSaveFile)
	{
		getSaveFile >> characterName;
	}
	getSaveFile.close();

	ofstream saveFile;
	saveFile.open(saveName);
	if (!saveFile)
	{
		cout << "\r Opening file failed!" << endl;
	}
	else
	{
		//characterName.erase(characterName.find_last_of("."), string::npos);
		saveFile << characterName << endl; /*Skip over character name at first line*/
		
		m_player->SaveInventory();
		//saveFile << variedItems << endl;
		for (int i = 0; i< m_player->m_itemList.size(); i++)
		{
			string saveItem = m_player->m_itemList[i];
			saveFile << saveItem << endl;
		}
		m_player->m_itemList.clear();
		//saveFile << "Kanoka Disc of Fire" << endl;
		/*Write player save data into here, following is placeholder data*/


		if (!saveFile) { cout << "Write failed!" << endl; }
		saveFile.close();
	}


	//saveFile.write("<player name>", 100);
}