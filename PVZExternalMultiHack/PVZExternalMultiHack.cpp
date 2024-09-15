#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

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

    // get handle to pvz
    HANDLE hPVZ{OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID)};

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
    }

    CloseHandle(hPVZ);
    return 0;
}
