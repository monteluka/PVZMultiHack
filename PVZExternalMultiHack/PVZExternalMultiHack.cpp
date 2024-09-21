#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#include "gameinfo.h"
#include "memory.h"

inline void toggleHack(const HANDLE& gameHandle, std::pair<bool, uintptr_t>& hackOption, const char* hackName, const char* newBytes, const size_t& newBytesLen, const char* oldBytes, const size_t& oldBytesLen);

int main()
{
    // get process id
    HANDLE hSnapshot{CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)};

    // need to get the size according to MSDN
    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // loop through all the processes in hSnapshot
    do
    {
        if (!_wcsicmp(pe32.szExeFile, L"popcapgame1.exe")) break;
    }
    while (Process32Next(hSnapshot, &pe32));
    CloseHandle(hSnapshot);

    // display information on the process
    std::wcout << "PVZ Process ID: " << pe32.th32ProcessID << '\n' << "Process name: " << pe32.szExeFile << '\n';

    // find pvz module base address
    HANDLE hModSnapshot{CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pe32.th32ProcessID)};

    // create moduleentry struct and initialize dwSize according to msdn
    MODULEENTRY32 me32{};
    me32.dwSize = sizeof(MODULEENTRY32);

    // loop through all the modules in hModSnapshot
    do
    {
        if (!_wcsicmp(me32.szModule, pe32.szExeFile)) break;
    }
    while (Module32Next(hModSnapshot, &me32));
    CloseHandle(hModSnapshot);

    // create GameInfo class for PVZ
    GameInfo infoPVZ{OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID), me32};

    // display module base address
    std::cout << "PVZ mod base address at: " << std::hex << infoPVZ.getModuleBaseAddress() << '\n';
    std::cout << "Auto addy is: " << std::hex << hacks.autoCollectItems.second << '\n';

    // start hack loop
    for (;;)
    {
        // skeleton for hacks
        if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('1') & 0x8000)
        {
            hacks.autoCollectItems.first = !hacks.autoCollectItems.first;
            if (hacks.autoCollectItems.first)
            {
                std::cout << "Autocollect items hack activated" << std::endl;
                WriteProcessMemory(infoPVZ.getRefToHandle(),
                                   reinterpret_cast<uintptr_t*>(hacks.autoCollectItems.second), "\xeb", 1, nullptr);
            }
            else
            {
                std::cout << "Autocollect items hack deactivated" << std::endl;;
                WriteProcessMemory(infoPVZ.getRefToHandle(),
                                   reinterpret_cast<uintptr_t*>(hacks.autoCollectItems.second), "\x75", 1, nullptr);
            }
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;
        Sleep(100);
    }

    return 0;
}

inline void toggleHack(const HANDLE& gameHandle, std::pair<bool, uintptr_t>& hackOption, const char* hackName, const char* newBytes, const size_t& newBytesLen, const char* oldBytes, const size_t& oldBytesLen)
{
	hackOption.first = !hackOption.first;
	if (hackOption.first)
	{
		std::cout << hackName << " hack activated" << std::endl;
		WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(hackOption.second), newBytes, newBytesLen, nullptr);
	}
	else
	{
		std::cout << hackName << " hack deactivated" << std::endl;;
		WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(hackOption.second), oldBytes, oldBytesLen, nullptr);
	}
}
