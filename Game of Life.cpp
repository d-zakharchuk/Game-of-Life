#include <iostream>
#include <fstream>
#include <ctime>
#include <windows.h>
#include <vector>
#include <string>
using namespace std;

const int Size = 65535;
const unsigned int G = 4294967295;
unsigned int FieldWidth, FieldHeight;
const char LiveCell = '#', DeadCell = '.';
unsigned long long Generations;
vector<bool>Field(1, 0);
vector<bool>TempField(1, 0);
char InputChoice;
bool SaveToFile = false, PrintField = true, ShowMode = false;
short Delay;

DWORD WINAPI CheckEscape(LPVOID lpParam)
{
	while (GetAsyncKeyState(VK_ESCAPE) == 0)
	{
	}
	exit(0);
}

void SetCursorPosition(short x, short y)
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x,y };
	SetConsoleCursorPosition(output, pos);
}

void cin_fail()
{
	cin.clear();
	char c[100];
	cin >> c;
}

void Configuration()
{
	cout << "Enter field width: ";
	cin >> FieldWidth;
	if (cin.fail())
	{
		cin_fail();
		FieldWidth = 10;
		cout << "Default value (10) was assigned.\n";
	}
	else if (FieldWidth<1 || FieldWidth>Size)
	{
		FieldWidth = 10;
		cout << "Default value (10) was assigned.\n";
	}
	cout << "Enter field height: ";
	cin >> FieldHeight;
	if (cin.fail())
	{
		cin_fail();
		FieldHeight = 10;
		cout << "Default value (10) was assigned.\n";
	}
	else if (FieldHeight<1 || FieldHeight>Size)
	{
		FieldHeight = 10;
		cout << "Default value (10) was assigned.\n";
	}
	cout << "Preparing field...\n";
	long long count = 1;
	while (count < FieldWidth*FieldHeight)
	{
		count++;
		Field.resize(count, 0);
		TempField.resize(count, 0);
		double percent = (count / (FieldWidth*FieldHeight / 100));
		if (count % 10000 == 0)
			cout << "Completed: " << count << "/" << FieldWidth*FieldHeight << " (" << percent << "%)\r";
	}
	cout << "\nField is ready.\n";
	cout << "Number of generations: ";
	cin >> Generations;
	if (cin.fail())
	{
		cin_fail();
		Generations = 3;
		cout << "Default value (3) was assigned.\n";
	}
	else if (Generations<1 || Generations>G)
	{
		Generations = 3;
		cout << "Default value (3) was assigned.\n";
	}
	cout << "Do you want to print the field?\n[  0  ] - no;\n[other] - yes.\nYour choice: ";
	cin >> InputChoice;
	if (InputChoice == '0')
		PrintField = false;
}

void SetStartGeneration()
{
	char line[Size + 1];
	for (long long y = 0; y < FieldHeight; y++)
	{
		cout << "Row " << y + 1 << " of " << FieldHeight << ": ";
		cin >> line;
		for (long long x = 0; x < FieldWidth; x++)
		{
			if (line[x] == '1')
				Field[x + y*FieldWidth] = 1;
			else
				Field[x + y*FieldWidth] = 0;
		}
	}
}

void Randomize()
{
	srand((unsigned)time(nullptr));
	for (long long y = 0; y < FieldHeight; y++)
		for (long long x = 0; x < FieldWidth; x++)
			Field[x + y*FieldWidth] = rand() % 2;
}

void GetStartGeneration()
{
	string Filename;
	cout << "Choose how to get start generation:\n[  0  ] - set manually in console;\n[  1  ] - read from file;\n"
		<< "[other] - randomize.\nYour choice: ";
	cin >> InputChoice;
	if (InputChoice == '0')
		SetStartGeneration();
	else if (InputChoice == '1')
	{
		cout << "Enter Filename: ";
		cin.ignore();
		getline(cin, Filename);
		ifstream MyFile(Filename);
		if (MyFile.fail())
		{
			cout << "No such file. Randomized instead\n";
			Randomize();
		}
		else
		{
			char line[Size + 1];
			for (long y = 0; y < FieldHeight; y++)
			{
				MyFile >> line;
				for (long x = 0; x < FieldWidth; x++)
				{
					if (line[x] == '1')
						Field[x + y*FieldWidth] = 1;
					else
						Field[x + y*FieldWidth] = 0;
				}
			}
		}
		MyFile.close();
	}
	else Randomize();
	cout << "Print results to: \n[ 0  ] - to file;\n[other] - to console\nYour choice: ";
	cin >> InputChoice;
	if (InputChoice == '0')
	{
		char OutputFilename[512];
		SaveToFile = true;
		cout << "Enter Filename: ";
		cin >> OutputFilename;
		FILE * stream;
		freopen_s(&stream, OutputFilename, "w", stdout);
		cout << "Field width: " << FieldWidth;
		cout << "\nField height: " << FieldHeight;
		cout << "\nGenerations: " << Generations;
		cout << "\nPrint field: " << PrintField << "\n";
	}
	else
	{
		cout << "Do you want to enable SHOW MODE?\n[  1  ] - yes;\n[other] - no.\nYour choice: ";
		cin >> InputChoice;
		if (InputChoice == '1')
		{
			ShowMode = true;
			cout << "Enter delay time between generations (in miliseconds): ";
			cin >> Delay;
			if (cin.fail())
			{
				cin_fail();
				Delay = 100;
				cout << "Default value (100) was assigned.\n";
			}
		}
		else
			cout << "\n";
	}
}

void PrintStartGeneration()
{
	if (ShowMode)
		system("cls");
	unsigned int CountLiveCells = 0;
	cout << "Generation: 0\n";
	for (long long y = 0; y < FieldHeight; y++)
	{
		for (long long x = 0; x < FieldWidth; x++)
		{
			if (Field[x + y*FieldWidth] == 1)
			{
				if (PrintField)
					cout << LiveCell;
				CountLiveCells++;
			}
			else if (PrintField)
				cout << DeadCell;
		}
		if (PrintField)
			cout << "\n";
	}
	if (ShowMode)
		cout << "Live cells: " << CountLiveCells << "                    ";
	else
		cout << "Live cells: " << CountLiveCells << "\n\n\n";
}

void PrintGenerations()
{
	for (unsigned int g = 0; g < Generations; g++)
	{
		if (ShowMode)
		{
			SetCursorPosition(0, 0);
			Sleep(Delay);
		}
		unsigned int CountLiveCells = 0;
		cout << "Generation: " << g + 1 << "\n";
		for (long long y = 0; y < FieldHeight; y++)
		{
			for (long long x = 0, iCellCounter = 0; x < FieldWidth; x++)
			{
				//#..
				//...
				//...
				if (((x - 1) >= 0) && ((y - 1) >= 0))
					if (Field[(x - 1) + (y - 1)*FieldWidth] == 1)
						iCellCounter++;
				//.#.
				//...
				//...
				if ((y - 1) >= 0)
					if (Field[x + (y - 1)*FieldWidth] == 1)
						iCellCounter++;
				//..#
				//...
				//...
				if (((x + 1) <= FieldWidth - 1) && ((y - 1) >= 0))
					if (Field[(x + 1) + (y - 1)*FieldWidth] == 1)
						iCellCounter++;
				//...
				//#..
				//...
				if ((x - 1) >= 0)
					if (Field[(x - 1) + y*FieldWidth] == 1)
						iCellCounter++;
				//...
				//..#
				//...
				if ((x + 1) <= FieldWidth - 1)
					if (Field[(x + 1) + y*FieldWidth] == 1)
						iCellCounter++;
				//...
				//...
				//#..
				if (((x - 1) >= 0) && ((y + 1) <= FieldHeight - 1))
					if (Field[(x - 1) + (y + 1)*FieldWidth] == 1)
						iCellCounter++;
				//...
				//...
				//.#.
				if ((y + 1) <= FieldHeight - 1)
					if (Field[x + (y + 1)*FieldWidth] == 1)
						iCellCounter++;
				//...
				//...
				//..#
				if (((x + 1) <= FieldWidth - 1) && ((y + 1) <= FieldHeight - 1))
					if (Field[(x + 1) + (y + 1)*FieldWidth] == 1)
						iCellCounter++;
				if ((iCellCounter<2) || iCellCounter>3)
				{
					TempField[x + y*FieldWidth] = 0;
					if (PrintField)
						cout << DeadCell;
				}
				else
				{
					if ((Field[x + y*FieldWidth]) == 0 && (iCellCounter != 3))
					{
						TempField[x + y*FieldWidth] = 0;
						if (PrintField)
							cout << DeadCell;
					}
					else
					{
						TempField[x + y*FieldWidth] = 1;
						if (PrintField)
							cout << LiveCell;
					}
				}
				iCellCounter = 0;
			}
			if (PrintField)
				cout << "\n";
		}
		for (long long y = 0; y < FieldHeight; y++)
			for (unsigned int x = 0; x < FieldWidth; x++)
			{
				Field[x + y*FieldWidth] = TempField[x + y*FieldWidth];
				if (Field[x + y*FieldWidth] == 1)
					CountLiveCells++;
			}
		if (ShowMode)
			cout << "Live cells: " << CountLiveCells << "                    ";
		else
			cout << "Live cells: " << CountLiveCells << "\n\n\n";
	}
}

int main()
{
	CreateThread(NULL, 0, CheckEscape, NULL, 0, NULL);
	Configuration();
	GetStartGeneration();
	PrintStartGeneration();
	PrintGenerations();
	if (!SaveToFile)
		system("Pause");
	return 0;
}
