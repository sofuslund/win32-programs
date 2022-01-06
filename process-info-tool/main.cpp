#include <iostream>
#include <string>
#include <windows.h>
#include <psapi.h>

#ifndef UNICODE
#define UNICODE
#endif 

using namespace std;

wstring getProcessName(DWORD processID)
{
    wchar_t processName[MAX_PATH] = L"<unknown>";

    // Get a handle to the process.
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    // Get the process name.
    if (processHandle != NULL)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModulesEx(processHandle, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_ALL))
        {
            GetModuleBaseNameW(processHandle, hMod, processName, sizeof(processName) / sizeof(wchar_t));
        }

        // Release the handle to the process.
        CloseHandle(processHandle);
    }

    // Return the process name converted to a wide string.
    return wstring(processName);
}

bool findProcess(wstring name, DWORD* id)
{
    // Get the list of process identifiers.
    DWORD processIDs[1024], cbNeeded;
    if (!EnumProcesses(processIDs, sizeof(processIDs), &cbNeeded))
    {
        return false;
    }
    // Calculate how many process identifiers were returned.
    DWORD cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.
    for (unsigned int i = 0; i < cProcesses; i++)
    {
        if (processIDs[i] != 0)
        {
            wstring processName = getProcessName(processIDs[i]);
            if (name == processName)
            {
                *id = processIDs[i];
                return true;
            }
        }
    }
    return false;
}

void listProcesses()
{
    // Get the list of process identifiers.
    DWORD processIDs[1024], cbNeeded;
    if (!EnumProcesses(processIDs, sizeof(processIDs), &cbNeeded))
    {
        cout << "Unable to list processes. " << endl;
    }
    // Calculate how many process identifiers were returned.
    DWORD cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.
    for (unsigned int i = 0; i < cProcesses; i++)
    {
        if (processIDs[i] != 0)
        {
            wstring processName = getProcessName(processIDs[i]);
            if (processName != L"<unknown>")
            {
                wcout << "Name: " << processName << " ID: " << processIDs[i] << endl;
            }
        }
    }
}

void trim(string& s)
{
    while (s[0] == ' ' || s[0] == '\f' || s[0] == '\t' || s[0] == '\v')
    {
        s.erase(s.begin());
    }
    int l = s.length() - 1;
    while (s[l] == ' ' || s[l] == '\f' || s[l] == '\t' || s[l] == '\v')
    {
        s.erase(s.begin() + l);
        l = s.length() - 1;
    }
}

int main(int argc, char* argv[])
{
    cout << "+-.+-.+-. process information tool .-+.-+.-+" << endl;
    cout << "Type help for a list of commands." << endl;
    cout << "Type quit to quit the program." << endl;

    DWORD id;

    while (true)
    {
        cout << "---> ";

        // Get user input.
        string command;
        cin >> command;
        string args;
        getline(cin, args);
        // Get rid of leading and trailing spaces from getline.
        trim(args);

        if (command == "help")
        {
            cout << "clear              clears the console screen" << endl;
            cout << "find_process       finds a process and gets it's id" << endl;
            cout << "help               shows list of commands" << endl;
            cout << "list_processes     shows a list of all processes" << endl;
            cout << "quit               quits the process injector tool" << endl;
        }
        else if (command == "list_processes")
        {
            listProcesses();
        }
        else if (command == "find_process")
        {

            string name = args;
            if (name == "")
            {
                cout << "Usage: find_process <name>" << endl;
            }
            else if (!findProcess(wstring(name.begin(), name.end()), &id))
            {
                cout << "Unable to find process " << name << "a" << endl;
            }
            else
            {
                cout << "Found process " << name << " with process id "<< id << endl;
            }
        }
        else if (command == "new_search")
        {

        }
        else if (command == "next_search")
        {

        }
        else if (command == "clear")
        {
            system("cls");
        }
        else if (command == "quit")
        {
            return 0;
        }
        else
        {
            cout << "Unknown command. Type help for a list of commands." << endl;
        }
    }
    return 0;
}
