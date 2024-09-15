#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "memory.h"

// global options
struct sHacks
{
    bool autoCollectItems{false};
    bool bypassSunLimit{false};
    bool fastSunProduction{false};
    bool hitAnywhere{false};
    bool infiniteCoins{false};
    bool infiniteLawnMower{false};
    bool infiniteSun{false};
    bool infinitePlantHealth{false};
    bool instantActivatePotatoMine{false};
    bool instantPlantRecharge{false};
    bool noChomperCooldown{false};
    bool noPlantRestriction{false};
    bool noZombies{false};
    bool oneHitKills{false};
} hacks;

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
    HANDLE hModSnapshot {CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pe32.th32ProcessID)};
    uintptr_t pvzBaseAddr {};

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

    // module has been found
    pvzBaseAddr = reinterpret_cast<uintptr_t>(me32.modBaseAddr);

    // display module base address
    std::cout << "PVZ mod base address at: " << std::hex << pvzBaseAddr << '\n';

    // get handle to pvz
    HANDLE hPVZ{OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID)};

    // test aob scan with autocollect items
    DWORD autoAddy {SigScan(hPVZ, me32, std::vector<int>{0x75, -1, 0x8b, 0xfb, 0xe8, -1, -1, -1, -1, 0xeb})};
    std::cout << "Auto addy is: " << std::hex << autoAddy << '\n';
    
    // start hack loop
    for (;;)
    {
        // skeleton for hacks
        if (hacks.autoCollectItems)
        {
        }
        else
        {
        }
        if (hacks.bypassSunLimit)
        {
        }
        else
        {
        }
        if (hacks.fastSunProduction)
        {
        }
        else
        {
        }
        if (hacks.hitAnywhere)
        {
        }
        else
        {
        }
        if (hacks.infiniteCoins)
        {
        }
        else
        {
        }
        if (hacks.infiniteLawnMower)
        {
        }
        else
        {
        }
        if (hacks.infiniteSun)
        {
        }
        else
        {
        }
        if (hacks.instantActivatePotatoMine)
        {
        }
        else
        {
        }
        if (hacks.infinitePlantHealth)
        {
        }
        else
        {
        }
        if (hacks.instantPlantRecharge)
        {
        }
        else
        {
        }
        if (hacks.noChomperCooldown)
        {
        }
        else
        {
        }
        if (hacks.noPlantRestriction)
        {
        }
        else
        {
        }
        if (hacks.noZombies)
        {
        }
        else
        {
        }
        if (hacks.oneHitKills)
        {
        }
        else
        {
        }
        break;
    }

    CloseHandle(hPVZ);
    return 0;
}
