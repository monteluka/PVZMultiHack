#include <array>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#include "gameinfo.h"
#include "memory.h"

inline void toggleHack(const HANDLE& gameHandle, std::pair<bool, uintptr_t>& hackOption, const char* hackName,
                       const char* newBytes, const char* oldBytes, const size_t& bytesLen);

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

    // allocate memory for hacks with extra instructions
    uintptr_t fastSunProductionAllocatedMem{
        reinterpret_cast<uintptr_t>(VirtualAllocEx(infoPVZ.getRefToHandle(), nullptr, 1000,
                       MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE))
    };

    // get the jump to alloc instructions
    std::array<BYTE, 7> fastSunProductionHookInstruction {"\xe9\xff\xff\xff\xff\x90"};
    uintptr_t offsetToAllocatedMem {fastSunProductionAllocatedMem - hacks.fastSunProduction.second - 5};
    memcpy(&fastSunProductionHookInstruction[1], &offsetToAllocatedMem, sizeof(uintptr_t));

    // get new code
    std::array<BYTE, 19> fastSunProductionJumpBackInstruction {"\xC7\x47\x58\x01\x00\x00\x00\xFF\x4F\x58\x8B\x77\x58\xE9\xFF\xFF\xFF\xFF"};
    // calculate the offset to original jump instruction
    uintptr_t jumpBackAddress {hacks.fastSunProduction.second - fastSunProductionAllocatedMem - 18 + 6};
    memcpy(&fastSunProductionJumpBackInstruction[14], &jumpBackAddress, sizeof(uintptr_t));
    WriteProcessMemory(infoPVZ.getRefToHandle(), reinterpret_cast<uintptr_t*>(fastSunProductionAllocatedMem), &fastSunProductionJumpBackInstruction, 18, nullptr);

    // remove allocated memory
    VirtualFreeEx(infoPVZ.getRefToHandle(), reinterpret_cast<uintptr_t*>(fastSunProductionAllocatedMem), 0, MEM_RELEASE);

    // start hack loop
    for (;;)
    {
        // skeleton for hacks
        if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('1') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.autoCollectItems, "Auto collect items", "\xeb", "\x75", 1);
        }
        if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('2') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.bypassSunLimit, "Bypass sun limit", "\xeb", "\x7e", 1);
        }
        if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('3') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.fastSunProduction, "Fast sun production", reinterpret_cast<char*>(&fastSunProductionHookInstruction),
                       "\xff\x4f\x58\x8b\x77\x58", 6);
            // requires code cave - not done
        }
        if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('4') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.instantHit, "Instant hit", "\x90\x90", "\x7c\x1b", 2);
        }
        if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('5') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.infiniteCoins, "Infinite Coins", "", "", 0); // need to find addy
        }
        if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('6') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.infiniteLawnMower, "Infinite lawn mower",
                       "\x90\x90\x90\x90\x90\x90\x90", "\xc7\x46\x2c\x02\x00\x00\x00", 7);
        }
        if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('7') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.infiniteSun, "Infinite Sun", "", "", 0);
            // need to find code cave
        }
        if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('8') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.infinitePlantHealth, "Infinite plant health", "", "", 0);
            // multi address
        }
        if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('9') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.instantActivatePotatoMine, "Instant activate potato mine",
                       "\x90\x90\x90\x90\x90\x90",
                       "\x0f\x85\xfd\x01\x00\x00", 6);
        }
        if (GetKeyState(VK_MENU) & 0x8000 && GetKeyState('1') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.instantPlantRecharge, "Instant plant recharge", "", "", 0);
            // need code cave
        }
        if (GetKeyState(VK_MENU) & 0x8000 && GetKeyState('2') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.noChomperCooldown, "No chomper cooldown", "\x90\x90", "\x75\x5f",
                       2);
        }
        if (GetKeyState(VK_MENU) & 0x8000 && GetKeyState('3') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.plantAnywhere, "Plant anywhere", "\x0F\x84",
                       "\x0F\x85", 2);
        }
        if (GetKeyState(VK_MENU) & 0x8000 && GetKeyState('4') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.noZombies, "No zombies", "", "", 0); // need to find code cave
        }
        if (GetKeyState(VK_MENU) & 0x8000 && GetKeyState('5') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.oneHitKills, "One hit kills", "", "", 0);
            // multiple addresses and need to find code cave
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;
        Sleep(100);
    }

    return 0;
}

inline void toggleHack(const HANDLE& gameHandle, std::pair<bool, uintptr_t>& hackOption, const char* hackName,
                       const char* newBytes, const char* oldBytes, const size_t& bytesLen)
{
    hackOption.first = !hackOption.first;
    if (hackOption.first)
    {
        std::cout << hackName << " hack activated" << std::endl;
        WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(hackOption.second), newBytes, bytesLen, nullptr);
    }
    else
    {
        std::cout << hackName << " hack deactivated" << std::endl;;
        WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(hackOption.second), oldBytes, bytesLen, nullptr);
    }
}
