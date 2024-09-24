#include <array>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#include "gameinfo.h"
#include "memory.h"

inline void toggleHack(const HANDLE& gameHandle, std::pair<bool, uintptr_t>& hackOption, const char* hackName,
                       const char* newBytes, const char* oldBytes, const size_t& bytesLen);
inline void toggleHackWithHook(const HANDLE& gameHandle, std::pair<bool, uintptr_t>& hackOption, sHookInfo& hookInfo,
                               const char* hackName, const char* oldBytes,
                               const size_t& bytesLen);

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

    // instantiate hook info structs for hacks that require custom code to run
    sHookInfo hiFastSunProduction("\xe9\xff\xff\xff\xff\x90", 6,
                                  "\xC7\x47\x58\x01\x00\x00\x00\xFF\x4F\x58\x8B\x77\x58\xE9\xFF\xFF\xFF\xFF", 18);
    sHookInfo hiInfiniteSun("\xe9\xff\xff\xff\xff\x90", 6,
                            "\xC7\x87\x78\x55\x00\x00\x0F\x27\x00\x00\x8B\x87\x78\x55\x00\x00\xE9\xFF\xFF\xFF\xFF", 21);
    sHookInfo hiInstantPlantRecharge("\xe9\xff\xff\xff\xff\x90", 6,
                                     "\x81\x47\x24\x00\x02\x00\x00\x8B\x47\x24\xE9\xFF\xFF\xFF\xFF", 15);
    sHookInfo hiNoZombies("\xe9\xff\xff\xff\xff\x90", 6,
                          "\xB8\x00\x00\x00\x00\xC1\xE0\x10\xE9\xFF\xFF\xFF\xFF", 13);

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
            toggleHackWithHook(infoPVZ.getRefToHandle(), hacks.fastSunProduction, hiFastSunProduction,
                               "Fast sun production",
                               "\xFF\x4F\x58\x8B\x77\x58", 6);
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
            toggleHackWithHook(infoPVZ.getRefToHandle(), hacks.infiniteSun, hiInfiniteSun, "Infinite sun",
                               "\x8B\x87\x78\x55\x00\x00", 6);
        }
        if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('8') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.infinitePlantHealth, "Infinite plant health", "", "", 0);
            // multi address - not yet implemented
        }
        if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('9') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.instantActivatePotatoMine, "Instant activate potato mine",
                       "\x90\x90\x90\x90\x90\x90",
                       "\x0f\x85\xfd\x01\x00\x00", 6);
        }
        if (GetKeyState(VK_MENU) & 0x8000 && GetKeyState('1') & 0x8000)
        {
            toggleHackWithHook(infoPVZ.getRefToHandle(), hacks.instantPlantRecharge, hiInstantPlantRecharge,
                               "Instant plant recharge", "\xFF\x47\x24\x8B\x47\x24", 6);
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
            toggleHackWithHook(infoPVZ.getRefToHandle(), hacks.noZombies, hiNoZombies, "No zombies",
                               "\x8B\x46\x14\xC1\xE0\x10", 6);
        }
        if (GetKeyState(VK_MENU) & 0x8000 && GetKeyState('5') & 0x8000)
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.oneHitKills, "One hit kills", "", "", 0);
            // multi address - not yet implemented
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

void toggleHackWithHook(const HANDLE& gameHandle, std::pair<bool, uintptr_t>& hackOption, sHookInfo& hookInfo,
                        const char* hackName, const char* oldBytes, const size_t& bytesLen)
{
    hackOption.first = !hackOption.first;
    if (hackOption.first)
    {
        // allocate memory for new instructions
        hookInfo.locAllocatedMemory = reinterpret_cast<uintptr_t>(VirtualAllocEx(
            gameHandle, nullptr, 1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

        // calculate offset to allocated memory from original instruction
        uintptr_t offsetToAllocatedMem{hookInfo.locAllocatedMemory - hackOption.second - 5};
        // now add that offset after jmp instruction that is used where we hook from
        memcpy(hookInfo.hookInstruction.data() + 1, &offsetToAllocatedMem, sizeof(uintptr_t));

        // calculate offset back to original hook location from location of newly allocated memory
        uintptr_t jumpBackAddress{
            hackOption.second - hookInfo.locAllocatedMemory - hookInfo.jumpBackInstruction.size() + hookInfo.
            hookInstruction.size()
        };
        // add that offset right after jmp instruction that will be used in newly allocated memory
        memcpy(hookInfo.jumpBackInstruction.data() + (hookInfo.jumpBackInstruction.size() - 4), &jumpBackAddress,
               sizeof(uintptr_t));

        // now write new instructions + jump back instruction to the newly allocated memory
        WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(hookInfo.locAllocatedMemory),
                           hookInfo.jumpBackInstruction.data(), hookInfo.jumpBackInstruction.size(), nullptr);


        std::cout << hackName << " hack activated" << std::endl;
        WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(hackOption.second), hookInfo.hookInstruction.data(),
                           bytesLen, nullptr);
    }
    else
    {
        // release the memory we allocated when activating the hack
        VirtualFreeEx(gameHandle, reinterpret_cast<uintptr_t*>(hookInfo.locAllocatedMemory), 0,
                      MEM_RELEASE);
        hookInfo.locAllocatedMemory = NULL;
        std::cout << hackName << " hack deactivated" << std::endl;;
        WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(hackOption.second), oldBytes, bytesLen, nullptr);
    }
}
