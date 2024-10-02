#include "gameinfo.h"

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
    if (_wcsicmp(pe32.szExeFile, gameName)) throw std::runtime_error(
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

    // ====================== single address hacks ===========================
    std::get<uintptr_t>(hacks.autoCollectItems) = SigScan(m_hPVZ, m_mPVZ32, {
                                                              0x75, 0xFFFF, 0x8B, 0xFFFF, 0xE8, 0xFFFF, 0xFFFF, 0xFFFF,
                                                              0xFFFF, 0xEB,
                                                              0xFFFF, 0x8B, 0xFFFF, 0xE8, 0xFFFF, 0xFFFF, 0xFFFF,
                                                              0xFFFF, 0x83
                                                          });
    std::get<uintptr_t>(hacks.bypassSunLimit) = SigScan(m_hPVZ, m_mPVZ32, {
                                                            0x7E, 0xFFFF, 0xC7, 0x80, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
                                                            0xFFFF, 0xFFFF,
                                                            0xFFFF, 0xFFFF, 0x81
                                                        });
    std::get<uintptr_t>(hacks.fastSunProduction) = SigScan(m_hPVZ, m_mPVZ32, {0xFF, 0x4F, 0x58, 0x8B, 0x77, 0x58});
    std::get<uintptr_t>(hacks.instantHit) = SigScan(m_hPVZ, m_mPVZ32, {0x7C, 0x1B, 0x83, 0x7C, 0x24, 0x18, 0x00});
    std::get<uintptr_t>(hacks.infiniteCoins) = SigScan(m_hPVZ, m_mPVZ32, {
                                                           0x8B, 0x51, 0x54, 0x52, 0x8D, 0x44, 0x24, 0x30
                                                       });
    std::get<uintptr_t>(hacks.infiniteLawnMower) = SigScan(m_hPVZ, m_mPVZ32, {
                                                               0xc7, 0x46, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x5f,
                                                               0x83, 0xc4
                                                           });
    std::get<uintptr_t>(hacks.infiniteSun) = SigScan(m_hPVZ, m_mPVZ32, {0x8B, 0x87, 0x78, 0x55, 0x00, 0x00});
    std::get<uintptr_t>(hacks.instantActivatePotatoMine) = SigScan(m_hPVZ, m_mPVZ32, {
                                                                       0x0f, 0x85, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8b,
                                                                       0x47, 0xFFFF,
                                                                       0x6a, 0xFFFF, 0x50
                                                                   });
    std::get<uintptr_t>(hacks.instantPlantRecharge) = SigScan(m_hPVZ, m_mPVZ32, {0xFF, 0x47, 0x24, 0x8B, 0x47, 0x24});
    std::get<uintptr_t>(hacks.noChomperCooldown) = SigScan(m_hPVZ, m_mPVZ32, {0x75, 0x5F, 0x6A, 0x14, 0x56});
    std::get<uintptr_t>(hacks.plantAnywhere) = SigScan(m_hPVZ, m_mPVZ32, {0x0F, 0x85, 0xA9, 0x04, 0x00, 0x00});
    std::get<uintptr_t>(hacks.noZombies) = SigScan(m_hPVZ, m_mPVZ32, {
                                                       0x8B, 0x46, 0x14, 0xC1, 0xE0, 0x10, 0x0B, 0xC3, 0x89, 0x87, 0x64
                                                   });

    // ====================== multi address hacks ===========================
    std::get<std::vector<uintptr_t>>(hacks.infinitePlantHealth).push_back(
        SigScan(m_hPVZ, m_mPVZ32, {0x29, 0x50, 0x40, 0x83, 0xF9, 0x19}));
    std::get<std::vector<uintptr_t>>(hacks.infinitePlantHealth).push_back(
        SigScan(m_hPVZ, m_mPVZ32, {0x83, 0x46, 0x40, 0xFC, 0x8B, 0x4E, 0x40}));

    std::get<std::vector<uintptr_t>>(hacks.oneHitKills).push_back(
        SigScan(m_hPVZ, m_mPVZ32, {0x8B, 0xAF, 0xC8, 0x00, 0x00, 0x00, 0x8B}));
    std::get<std::vector<uintptr_t>>(hacks.oneHitKills).push_back(
        SigScan(m_hPVZ, m_mPVZ32, {0x8B, 0x8D, 0xD0, 0x00, 0x00, 0x00, 0xB8}));
    std::get<std::vector<uintptr_t>>(hacks.oneHitKills).push_back(
        SigScan(m_hPVZ, m_mPVZ32, {0x29, 0x86, 0xDC, 0x00, 0x00, 0x00}));
}
