﻿#pragma once
#include "memory.h"

// bool will determine if hack is enabled or enabled
// uinptr_t holds the location in memory that stores the bytes we want to modify for the hack
inline struct sHacks
{
    std::pair<bool, uintptr_t> autoCollectItems{false, 0};
    std::pair<bool, uintptr_t> bypassSunLimit{false, 0};
    std::pair<bool, uintptr_t> fastSunProduction{false, 0};
    std::pair<bool, uintptr_t> hitAnywhere{false, 0};
    std::pair<bool, uintptr_t> infiniteCoins{false, 0};
    std::pair<bool, uintptr_t> infiniteLawnMower{false, 0}; // multi address
    std::pair<bool, uintptr_t> infiniteSun{false, 0};
    std::pair<bool, uintptr_t> infinitePlantHealth{false, 0}; // multi address 
    std::pair<bool, uintptr_t> instantActivatePotatoMine{false, 0};
    std::pair<bool, uintptr_t> instantPlantRecharge{false, 0};
    std::pair<bool, uintptr_t> noChomperCooldown{false, 0};
    std::pair<bool, uintptr_t> noPlantRestriction{false, 0};
    std::pair<bool, uintptr_t> noZombies{false, 0};
    std::pair<bool, uintptr_t> oneHitKills{false, 0}; // multi address
} hacks;

class GameInfo
{
public:
    GameInfo(HANDLE hPVZ, MODULEENTRY32 mPVZ32);
    ~GameInfo();

    [[nodiscard]] const HANDLE& getRefToHandle() const { return m_hPVZ; }
    [[nodiscard]] uintptr_t getModuleBaseAddress() const { return reinterpret_cast<uintptr_t>(m_mPVZ32.modBaseAddr); }

private:
    HANDLE m_hPVZ{};
    MODULEENTRY32 m_mPVZ32{};
};
