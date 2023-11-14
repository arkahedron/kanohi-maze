#include <iostream>

using namespace std;

constexpr int kHeight = 4;
constexpr int kWidth = 4;

void DisplayMap(char map[kHeight][kWidth]);

int main()
{
	char myMap[kHeight][kWidth] = { {'+', '-', '-', '+'},
									{'|', '@', ' ', '|'},
									{'|', ' ', ' ', '|'},
									{'+', '-', '-','+'},
								};
	DisplayMap(myMap);
}

void DisplayMap(char map[kHeight][kWidth])
{
	for (int y = 0; y < kHeight; y++)
	{
		for (int x = 0; x < kWidth; x++)
		{
			cout << map[y][x];
		}
		cout << endl;
	}
}