#include "MainMenuState.h"

#include <iostream>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <vector>

#include "StateMachineCore.h"

using namespace std;

constexpr unsigned int kEscapeKey = 27;
constexpr unsigned int kEnterKey = 13;
constexpr unsigned int kUpKey = 72;
constexpr unsigned int kDownKey = 80;

constexpr char kLoadSave = '1';
constexpr char kNewSave = '2';
constexpr char kQuit = '3';

constexpr unsigned char WAL = (char)219;

MainMenuState::MainMenuState(StateMachineCore* pOwner)
	: m_pOwner(pOwner)
{

}

bool MainMenuState::Update(bool processInput)
{
	bool shouldQuit = false;
	int menuSelection = -1;
	if (processInput)
	{
		menuSelection = MenuSelector();

		switch (menuSelection)
		{
		case 0: /*Load Save*/
		{
			//m_visuals.WipeLastLines(0);

			string savefileToLoad = FindSaveFiles();
			if (LoadSaveFile(savefileToLoad) == true)
			{
				cout << endl << " Loading Save...";
				Sleep(900);
				m_pOwner->LoadScene(StateMachineCore::SceneName::Archives);
			}
		} break;
		case 1: /*New Game*/
		{
			//m_visuals.WipeLastLines(0);

			string savefileToLoad = CreateSaveFile();
			if (LoadSaveFile(savefileToLoad) == true)
			{
				cout << endl << " Starting New Game...";
				Sleep(900);
				m_pOwner->LoadScene(StateMachineCore::SceneName::Archives);
			}
		} break;
		case 2: /*Exit Game*/
		{
			//m_visuals.WipeLastLines(0);
			cout << endl << " Exiting Game..." << endl;
			shouldQuit = true;
		} break;
		default:
			break;
		}
	}
	
	return shouldQuit;
}

void MainMenuState::Draw()
{

}

int MainMenuState::MenuSelector()
{
	string Menu[3] = { "Load Save", "New  Game", "Exit Game" };
	int pick = 0;
	int selectedOption = pick;
	bool hasSelected = false;
	while (!hasSelected)
	{
		system("cls");
		m_visuals.ResetTextColor();
		m_visuals.ResetCursor();
		cout << "\n";
		cout << "  " << string(15, WAL) << endl;
		cout << "  " << WAL << "[KANOHI MAZE]" << WAL << endl;
		cout << "  " << string(15, WAL) << endl;
		cout << "  " << string(2, WAL) << " MAIN MENU " << string(2, WAL) << endl;
		cout << "  " << string(15, WAL) << endl;
		cout << "  " << string(15, WAL) << endl;

		for (int i = 0; i < 3; ++i)
		{
			cout << "  " << string(2, WAL);
			if (i == pick) {
				m_visuals.ColorText(AColor::Inverted);
				cout << ">" << Menu[i] << " ";
			}
			else {
				m_visuals.ResetTextColor();
				cout << " " << Menu[i] << " ";
			}
			m_visuals.ResetTextColor();
			cout << string(2, WAL) << endl;
		}
		cout << "  " << string(15, WAL) << endl;

		while (true)
		{
			int input = _getch();
			if (input == 'w' || (char)input == kUpKey)
			{
				pick -= 1;
				if (pick == -1) { pick = 2; }
				break;
			}
			else if (input == 's' || (char)input == kDownKey)
			{
				pick += 1;
				if (pick == 3) { pick = 0; }
				break;
			}
			else if (input == 'e' || input == kEnterKey)
			{
				//cout << "\r";
				switch (pick)
				{
				case 0:
					if (m_input.BinaryChoice("Load Existing Save?")) { return 0; }
					break;
				case 1:
					if (m_input.BinaryChoice("Start New Game?")) { return 1; }
					break;
				case 2:
					if (m_input.BinaryChoice("Really Quit?")) { return 2; }
					break;
				default:
					break;
				}
				cout << "\r";
			}
		}
		Sleep(120);
	}
	return -1;
}



string MainMenuState::CreateSaveFile()
{
	string saveName;

	cout << "\r Name your character: ";
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
		saveFile << "Great Kanohi Huna" << endl;
		saveFile << "Kanoka Disc of Fire" << endl;
		//saveFile.write("<player name>", 100);



		if (!saveFile){ cout << "Write failed!" << endl; }
		saveFile.close();
	}
	return saveName;
}

string MainMenuState::FindSaveFiles()
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
		system("pause");
		return " ";
	}

	for(int i = 0; i < saveIndex; i++)
	{
		saveSelect = sfListArray[i];
		string saveListItem(saveSelect.begin(), saveSelect.end());
		saveListItem.erase(saveListItem.find_last_of("."), string::npos);
		cout << endl << " - " << saveListItem;
	}
	cout << endl << " Select which save file to load: ";

	string saveInput;
	cin >> saveInput;
	saveInput.append(".txt");
	return saveInput;


	//while (!fileListArray.empty())
	//{
	//	delete &fileListArray.back();
	//	fileListArray.pop_back();
	//}

	//string selectedSave;
	//int saveNum = 0;
	//
	//	cout << " What level would you like to load?" << endl;
	//
	//	//Search for and print out potential valid text files as saves
	//	WIN32_FIND_DATA FindFileData;
	//	HANDLE hFind = FindFirstFile(L"../Saves/*", &FindFileData);
	//	if (hFind != INVALID_HANDLE_VALUE) {
	//		do {
	//			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	//			{
	//				wstring wsLevel(FindFileData.cFileName);
	//				string saveStr(wsLevel.begin(), wsLevel.end());
	//				if (bool isTextFile = (saveStr.substr(saveStr.find_last_of(".") + 1) == "txt"))
	//				{
	//					saveStr.erase(saveStr.find_last_of("."), string::npos);
	//					saveNum++;
	//					cout << " > " << saveNum << ": " << saveStr << endl;
	//				}
	//			}
	//		} while (FindNextFile(hFind, &FindFileData) != 0);
	//		FindClose(hFind);
	//
	//		//Player input to select level, will be overhauled once map tethering is implemented
	//		string saveInput;
	//		cin >> saveInput;
	//		saveInput.append(".txt");
	//		return saveInput;
	//	}
	//	else { cout << " No valid levels found!" << endl; }
}

bool MainMenuState::LoadSaveFile(string saveName)
{
	int d_1;
	int d_2;
	//Get defined text file as level and try to open it
	saveName.insert(0, "../Saves/");
	ifstream saveFile;
	saveFile.open(saveName);
	if (saveFile)
	{
		constexpr int tempSize = 25;
		char temp[tempSize];

		saveFile.getline(temp, tempSize, '\n');
		d_1 = atoi(temp);

		saveFile.getline(temp, tempSize, '\n');
		d_2 = atoi(temp);

		char* saveData = new char[d_1 * d_2];
		saveFile.read(saveData, d_1 * d_2);

		return true;
	}
	else {
		cout << " Invalid save file, please try again!" << endl;
		Sleep(900);
		return false;
	}
}