﻿#include "gameinfo.h"

GameInfo::GameInfo(const wchar_t* gameName)
{
    // === get handle to the game ===
    // first get process id
    HANDLE hSnapshot{CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)};

    // need to get the size according to MSDN
    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // loop through all the processes in hSnapshot
    do
    {
        if (!_wcsicmp(pe32.szExeFile, gameName)) break;
    }
    while (Process32Next(hSnapshot, &pe32));
    CloseHandle(hSnapshot);

    // display information on the process
    std::wcout << "PVZ Process ID: " << pe32.th32ProcessID << '\n' << "Process name: " << pe32.szExeFile << '\n';

    // make sure that we have found the game running
    if (_wcsicmp(pe32.szExeFile, gameName) != 0)
        throw std::runtime_error(
            "Unable to find Plants vs. Zombies executable!\nMake sure the game is running.");

    // assign handle to member variable
    m_hPVZ = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

    // === now get module info for game ===
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


    // assign module base address to member variable
    m_mPVZ32 = me32;

    // display module base address
    std::cout << "PVZ mod base address at: " << std::hex << reinterpret_cast<uintptr_t>(m_mPVZ32.modBaseAddr) << '\n';

    // handle and module info for pvz have been assigned so now hacks can be populated
    populateHacks();
}

GameInfo::~GameInfo()
{
    CloseHandle(m_hPVZ);
}

bool GameInfo::isGameRunning() const
{
    DWORD ret{WaitForSingleObject(m_hPVZ, 0)};
    return ret == WAIT_TIMEOUT;
}

void GameInfo::populateHacks() const
{
    // populate all the addresses in hacks struct

    // entering hook address
    // ====================== single address hacks ===========================
    std::get<2>(hacks.autoCollectItems) = SigScan(m_hPVZ, m_mPVZ32, {
                                                      0x75, 0xFFFF, 0x8B, 0xFFFF, 0xE8, 0xFFFF, 0xFFFF, 0xFFFF,
                                                      0xFFFF, 0xEB,
                                                      0xFFFF, 0x8B, 0xFFFF, 0xE8, 0xFFFF, 0xFFFF, 0xFFFF,
                                                      0xFFFF, 0x83
                                                  });
    std::get<2>(hacks.bypassSunLimit) = SigScan(m_hPVZ, m_mPVZ32, {
                                                    0x7E, 0xFFFF, 0xC7, 0x80, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
                                                    0xFFFF, 0xFFFF,
                                                    0xFFFF, 0xFFFF, 0x81
                                                });
    std::get<2>(hacks.fastSunProduction) = SigScan(m_hPVZ, m_mPVZ32, {0xFF, 0x4F, 0x58, 0x8B, 0x77, 0x58});
    std::get<2>(hacks.instantHit) = SigScan(m_hPVZ, m_mPVZ32, {0x7C, 0x1B, 0x83, 0x7C, 0x24, 0x18, 0x00});
    std::get<2>(hacks.infiniteCoins) = SigScan(m_hPVZ, m_mPVZ32, {
                                                   0x8B, 0x51, 0x54, 0x52, 0x8D, 0x44, 0x24, 0x30
                                               });
    std::get<2>(hacks.infiniteLawnMower) = SigScan(m_hPVZ, m_mPVZ32, {
                                                       0xc7, 0x46, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x5f,
                                                       0x83, 0xc4
                                                   });
    std::get<2>(hacks.infiniteSun) = SigScan(m_hPVZ, m_mPVZ32, {0x8B, 0x87, 0x78, 0x55, 0x00, 0x00});
    std::get<2>(hacks.instantActivatePotatoMine) = SigScan(m_hPVZ, m_mPVZ32, {
                                                               0x0f, 0x85, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8b,
                                                               0x47, 0xFFFF,
                                                               0x6a, 0xFFFF, 0x50
                                                           });
    std::get<2>(hacks.instantPlantRecharge) = SigScan(m_hPVZ, m_mPVZ32, {0xFF, 0x47, 0x24, 0x8B, 0x47, 0x24});
    std::get<2>(hacks.noCobCannonCooldown) = SigScan(m_hPVZ, m_mPVZ32, {
                                                         0x0f, 0x85, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xd9, 0x05, 0xFFFF,
                                                         0xFFFF, 0xFFFF, 0xFFFF, 0x51, 0xd9, 0x1c, 0xFFFF, 0x6a, 0xFFFF,
                                                         0x8d, 0x48
                                                     });
    std::get<2>(hacks.noChomperCooldown) = SigScan(m_hPVZ, m_mPVZ32, {0x75, 0x5F, 0x6A, 0x14, 0x56});
    std::get<2>(hacks.noMagnetShroomCooldown) = SigScan(m_hPVZ, m_mPVZ32, {0x0f, 0x85, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xd9, 0x05, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x51, 0xd9, 0x1c, 0xFFFF, 0xc7, 0x43});
    std::get<2>(hacks.plantAnywhere) = SigScan(m_hPVZ, m_mPVZ32, {0x0F, 0x85, 0xA9, 0x04, 0x00, 0x00});
    std::get<2>(hacks.noZombies) = SigScan(m_hPVZ, m_mPVZ32, {
                                               0x8B, 0x46, 0x14, 0xC1, 0xE0, 0x10, 0x0B, 0xC3, 0x89, 0x87, 0x64
                                           });
    // ====================== multi address hacks ===========================
    std::get<2>(hacks.infinitePlantHealth).push_back(
        SigScan(m_hPVZ, m_mPVZ32, {0x29, 0x50, 0x40, 0x83, 0xF9, 0x19}));
    std::get<2>(hacks.infinitePlantHealth).push_back(
        SigScan(m_hPVZ, m_mPVZ32, {0x83, 0x46, 0x40, 0xFC, 0x8B, 0x4E, 0x40}));

    std::get<2>(hacks.oneHitKills).push_back(
        SigScan(m_hPVZ, m_mPVZ32, {0x8B, 0xAF, 0xC8, 0x00, 0x00, 0x00, 0x8B}));
    std::get<2>(hacks.oneHitKills).push_back(
        SigScan(m_hPVZ, m_mPVZ32, {0x8B, 0x8D, 0xD0, 0x00, 0x00, 0x00, 0xB8}));
    std::get<2>(hacks.oneHitKills).push_back(
        SigScan(m_hPVZ, m_mPVZ32, {0x29, 0x86, 0xDC, 0x00, 0x00, 0x00}));

    // entering new bytes for each hack
    // ====================== single address hacks ===========================
    std::get<3>(hacks.autoCollectItems) = "\xeb";
    std::get<3>(hacks.bypassSunLimit) = "\xeb";
    std::get<3>(hacks.fastSunProduction) = "\xe9\xff\xff\xff\xff\x90";
    std::get<3>(hacks.instantHit) = "\x90\x90";
    std::get<3>(hacks.infiniteCoins) = "\xe9\xff\xff\xff\xff\x90\x90\x90";
    std::get<3>(hacks.infiniteLawnMower) = "\x90\x90\x90\x90\x90\x90\x90";
    std::get<3>(hacks.infiniteSun) = "\xe9\xff\xff\xff\xff\x90";
    std::get<3>(hacks.instantActivatePotatoMine) = "\x90\x90\x90\x90\x90\x90";
    std::get<3>(hacks.instantPlantRecharge) = "\xe9\xff\xff\xff\xff\x90";
    std::get<3>(hacks.noCobCannonCooldown) = "\x90\x90\x90\x90\x90\x90";
    std::get<3>(hacks.noChomperCooldown) = "\x90\x90";
    std::get<3>(hacks.noMagnetShroomCooldown) = "\x90\x90\x90\x90\x90\x90";
    std::get<3>(hacks.plantAnywhere) = "\x0F\x84";
    std::get<3>(hacks.noZombies) = "\xe9\xff\xff\xff\xff\x90";
    // ====================== multi address hacks ===========================
    std::get<3>(hacks.infinitePlantHealth) = {"\x90\x90\x90", "\x90\x90\x90\x90"};
    std::get<3>(hacks.oneHitKills) = {
        "\xBD\x00\x00\x00\x00\x90", "\xB9\x00\x00\x00\x00\x90", "\x90\x90\x90\x90\x90\x90"
    };

    // entering the old bytes for each hack
    // ====================== single address hacks ===========================
    std::get<4>(hacks.autoCollectItems) = "\x75";
    std::get<4>(hacks.bypassSunLimit) = "\x7e";
    std::get<4>(hacks.fastSunProduction) = "\xFF\x4F\x58\x8B\x77\x58";
    std::get<4>(hacks.instantHit) = "\x7c\x1b";
    std::get<4>(hacks.infiniteCoins) = "\x8B\x51\x54\x52\x8D\x44\x24\x30";
    std::get<4>(hacks.infiniteLawnMower) = "\xc7\x46\x2c\x02\x00\x00\x00";
    std::get<4>(hacks.infiniteSun) = "\x8B\x87\x78\x55\x00\x00";
    std::get<4>(hacks.instantActivatePotatoMine) = "\x0f\x85\xfd\x01\x00\x00";
    std::get<4>(hacks.instantPlantRecharge) = "\xFF\x47\x24\x8B\x47\x24";
    std::get<4>(hacks.noCobCannonCooldown) = "\x0F\x85\x92\x01\x00\x00";
    std::get<4>(hacks.noChomperCooldown) = "\x75\x5f";
    std::get<4>(hacks.noMagnetShroomCooldown) = "\x0F\x85\x46\x05\x00\x00";
    std::get<4>(hacks.plantAnywhere) = "\x0F\x85";
    std::get<4>(hacks.noZombies) = "\x8B\x46\x14\xC1\xE0\x10";
    // ====================== multi address hacks ===========================
    std::get<4>(hacks.infinitePlantHealth) = {"\x29\x50\x40", "\x83\x46\x40\xFC"};
    std::get<4>(hacks.oneHitKills) = {
        "\x8B\xAF\xC8\x00\x00\x00", "\x8B\x8D\xD0\x00\x00\x00", "\x29\x86\xDC\x00\x00\x00"
    };

    // entering the size of the bytes
    // ====================== single address hacks ===========================
    std::get<5>(hacks.autoCollectItems) = 1;
    std::get<5>(hacks.bypassSunLimit) = 1;
    std::get<5>(hacks.fastSunProduction) = 6;
    std::get<5>(hacks.instantHit) = 2;
    std::get<5>(hacks.infiniteCoins) = 8;
    std::get<5>(hacks.infiniteLawnMower) = 7;
    std::get<5>(hacks.infiniteSun) = 6;
    std::get<5>(hacks.instantActivatePotatoMine) = 6;
    std::get<5>(hacks.instantPlantRecharge) = 6;
    std::get<5>(hacks.noCobCannonCooldown) = 6;
    std::get<5>(hacks.noChomperCooldown) = 2;
    std::get<5>(hacks.noMagnetShroomCooldown) = 6;
    std::get<5>(hacks.plantAnywhere) = 2;
    std::get<5>(hacks.noZombies) = 6;
    // ====================== multi address hacks ===========================
    std::get<5>(hacks.infinitePlantHealth) = {3, 4};
    std::get<5>(hacks.oneHitKills) = {6, 6, 6};
}

void disableAllHacks()
{
    if (std::get<1>(hacks.autoCollectItems))
    {
        std::get<0>(hacks.autoCollectItems) = true;
        std::get<1>(hacks.autoCollectItems) = false;
    }
    if (std::get<1>(hacks.bypassSunLimit))
    {
        std::get<0>(hacks.bypassSunLimit) = true;
        std::get<1>(hacks.bypassSunLimit) = false;
    }
    if (std::get<1>(hacks.fastSunProduction))
    {
        std::get<0>(hacks.fastSunProduction) = true;
        std::get<1>(hacks.fastSunProduction) = false;
    }
    if (std::get<1>(hacks.instantHit))
    {
        std::get<0>(hacks.instantHit) = true;
        std::get<1>(hacks.instantHit) = false;
    }
    if (std::get<1>(hacks.infiniteCoins))
    {
        std::get<0>(hacks.infiniteCoins) = true;
        std::get<1>(hacks.infiniteCoins) = false;
    }
    if (std::get<1>(hacks.infiniteLawnMower))
    {
        std::get<0>(hacks.infiniteLawnMower) = true;
        std::get<1>(hacks.infiniteLawnMower) = false;
    }
    if (std::get<1>(hacks.infiniteSun))
    {
        std::get<0>(hacks.infiniteSun) = true;
        std::get<1>(hacks.infiniteSun) = false;
    }
    if (std::get<1>(hacks.infinitePlantHealth))
    {
        std::get<0>(hacks.infinitePlantHealth) = true;
        std::get<1>(hacks.infinitePlantHealth) = false;
    }
    if (std::get<1>(hacks.instantActivatePotatoMine))
    {
        std::get<0>(hacks.instantActivatePotatoMine) = true;
        std::get<1>(hacks.instantActivatePotatoMine) = false;
    }
    if (std::get<1>(hacks.instantPlantRecharge))
    {
        std::get<0>(hacks.instantPlantRecharge) = true;
        std::get<1>(hacks.instantPlantRecharge) = false;
    }
    if (std::get<1>(hacks.noCobCannonCooldown))
    {
        std::get<0>(hacks.noCobCannonCooldown) = true;
        std::get<1>(hacks.noCobCannonCooldown) = false;
    }
    if (std::get<1>(hacks.noChomperCooldown))
    {
        std::get<0>(hacks.noChomperCooldown) = true;
        std::get<1>(hacks.noChomperCooldown) = false;
    }
    if (std::get<1>(hacks.noMagnetShroomCooldown))
    {
        std::get<0>(hacks.noMagnetShroomCooldown) = true;
        std::get<1>(hacks.noMagnetShroomCooldown) = false;
    }
    if (std::get<1>(hacks.plantAnywhere))
    {
        std::get<0>(hacks.plantAnywhere) = true;
        std::get<1>(hacks.plantAnywhere) = false;
    }
    if (std::get<1>(hacks.noZombies))
    {
        std::get<0>(hacks.noZombies) = true;
        std::get<1>(hacks.noZombies) = false;
    }
    if (std::get<1>(hacks.oneHitKills))
    {
        std::get<0>(hacks.oneHitKills) = true;
        std::get<1>(hacks.oneHitKills) = false;
    }
}
