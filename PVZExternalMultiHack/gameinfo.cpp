#include "gameinfo.h"


sHookInfo::sHookInfo(const char* hookInstructionBytes, const size_t& hookBytesLen, const char* jumpBackInstructionBytes,
    const size_t& jumpBackBytesLen)
{
    for (size_t i {0}; i < hookBytesLen; ++i)
    {
        hookInstruction.push_back(hookInstructionBytes[i]);
    }
    for (size_t i {0}; i < jumpBackBytesLen; ++i)
    {
        jumpBackInstruction.push_back(jumpBackInstructionBytes[i]);
    }
}

GameInfo::GameInfo(HANDLE hPVZ, MODULEENTRY32W mPVZ32) : m_hPVZ(hPVZ), m_mPVZ32(mPVZ32)
{
    // populate all the addresses in hacks struct
    hacks.autoCollectItems.second = SigScan(hPVZ, m_mPVZ32, {
                                                0x75, 0xFFFF, 0x8B, 0xFFFF, 0xE8, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xEB,
                                                0xFFFF, 0x8B, 0xFFFF, 0xE8, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x83
                                            });
    hacks.bypassSunLimit.second = SigScan(hPVZ, m_mPVZ32, {
                                              0x7E, 0xFFFF, 0xC7, 0x80, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
                                              0xFFFF, 0xFFFF, 0x81
                                          });
    hacks.fastSunProduction.second = SigScan(hPVZ, m_mPVZ32, {0xFF, 0x4F, 0x58, 0x8B, 0x77, 0x58});
    hacks.instantHit.second = SigScan(hPVZ, m_mPVZ32, {0x7C, 0x1B, 0x83, 0x7C, 0x24, 0x18, 0x00});
    hacks.infiniteCoins.second = SigScan(hPVZ, m_mPVZ32, {
                                             0x8B, 0x41, 0xFFFF, 0x8D, 0x7C, 0xFFFF, 0xFFFF, 0xE8, 0xFFFF, 0xFFFF,
                                             0xFFFF, 0xFFFF, 0xC7, 0xFFFF, 0xFFFF, 0xFFFF, 0x00, 0x00, 0x00, 0x00, 0x8B
                                         });
    hacks.infiniteLawnMower.second = SigScan(hPVZ, m_mPVZ32, {
                                                 0xc7, 0x46, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x5f, 0x83, 0xc4
                                             });
    hacks.infiniteSun.second = SigScan(hPVZ, m_mPVZ32, {0x8B, 0x87, 0x78, 0x55, 0x00, 0x00});
    hacks.infinitePlantHealth.second = SigScan(hPVZ, m_mPVZ32, {});
    hacks.instantActivatePotatoMine.second = SigScan(hPVZ, m_mPVZ32, {
                                                         0x0f, 0x85, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8b, 0x47, 0xFFFF,
                                                         0x6a, 0xFFFF, 0x50
                                                     });
    hacks.instantPlantRecharge.second = SigScan(hPVZ, m_mPVZ32, {0xFF, 0x47, 0x24, 0x8B, 0x47, 0x24});
    hacks.noChomperCooldown.second = SigScan(hPVZ, m_mPVZ32, {0x75, 0x5F, 0x6A, 0x14, 0x56});
    hacks.plantAnywhere.second = SigScan(hPVZ, m_mPVZ32, {0x0F, 0x85, 0xA9, 0x04, 0x00, 0x00});
    hacks.noZombies.second = SigScan(hPVZ, m_mPVZ32, {
                                         0x8B, 0x46, 0x14, 0xC1, 0xE0, 0x10, 0x0B, 0xC3, 0x89, 0x87, 0x64
                                     });
    hacks.oneHitKills.second = SigScan(hPVZ, m_mPVZ32, {});
}

GameInfo::~GameInfo()
{
    CloseHandle(m_hPVZ);
}
