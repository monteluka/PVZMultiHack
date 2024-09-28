#include <vector>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#include "gameinfo.h"
#include "memory.h"

int main()
{
    // instantiate GameInfo class for PVZ
    const GameInfo infoPVZ (L"popcapgame1.exe");

    // instantiate hook info structs for hacks that require custom code to run
    sHookInfo hiFastSunProduction("\xe9\xff\xff\xff\xff\x90", 6,
                                  "\xC7\x47\x58\x01\x00\x00\x00\xFF\x4F\x58\x8B\x77\x58\xE9\xFF\xFF\xFF\xFF", 18);
    sHookInfo hiInfiniteCoins("\xe9\xff\xff\xff\xff\x90\x90\x90", 8,
                              "\xC7\x41\x54\x9F\x86\x01\x00\x8B\x51\x54\x52\x8D\x44\x24\x30\xE9\xFF\xFF\xFF\xFF", 20);
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
            toggleHackWithHook(infoPVZ.getRefToHandle(), hacks.infiniteCoins, hiInfiniteCoins, "Infinite coins",
                               "\x8B\x51\x54\x52\x8D\x44\x24\x30", 8);
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
            toggleHack(infoPVZ.getRefToHandle(), hacks.infinitePlantHealth, "Infinite plant health",
                       {"\x90\x90\x90", "\x90\x90\x90\x90"}, {"\x29\x50\x40", "\x83\x46\x40\xFC"}, {3, 4});
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
            toggleHack(infoPVZ.getRefToHandle(), hacks.oneHitKills, "One hit kills",
                       {"\xBD\x00\x00\x00\x00\x90", "\xB9\x00\x00\x00\x00\x90", "\x90\x90\x90\x90\x90\x90"}, {
                           "\x8B\xAF\xC8\x00\x00\x00", "\x8B\x8D\xD0\x00\x00\x00", "\x29\x86\xDC\x00\x00\x00"
                       }, {6, 6, 6});
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;
        Sleep(100);
    }

    return 0;
}
