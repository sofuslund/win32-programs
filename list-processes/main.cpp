#include <windows.h>
#include <psapi.h>
#include <iostream>

#pragma comment (lib, "Psapi.lib")

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

void listProcesses()
{
    // Get the list of process identifiers.
    DWORD processIDs[1024], cbNeeded;
    if (!EnumProcesses(processIDs, sizeof(processIDs), &cbNeeded))
    {
        cout << "An error ocurred" << endl;
        return;
    }
    // Calculate how many process identifiers were returned.
    DWORD cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.
    for (unsigned int i = 0; i < cProcesses; i++)
    {
        if (processIDs[i] != 0)
        {
            wstring name = getProcessName(processIDs[i]);
            if (name != L"<unknown>")
                wcout << name << endl;
        }
    }
}

int main()
{
    listProcesses();
    return 0;
}
