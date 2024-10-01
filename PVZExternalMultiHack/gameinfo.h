#pragma once
#include "memory.h"
#include <tuple>

// <0> holds the "was checked" value for the menu
// <1> will determine if hack is enabled or enabled
// <2> holds the location in memory that stores the bytes we want to modify for the hack
inline struct sHacks
{
    std::tuple<bool, bool, uintptr_t> autoCollectItems{false, false, 0};
    std::tuple<bool, bool, uintptr_t> bypassSunLimit{false, false, 0};
    std::tuple<bool, bool, uintptr_t> fastSunProduction{false, false, 0};
    std::tuple<bool, bool, uintptr_t> instantHit{false, false, 0};
    std::tuple<bool, bool, uintptr_t> infiniteCoins{false, false, 0};
    std::tuple<bool, bool, uintptr_t> infiniteLawnMower{false, false, 0};
    std::tuple<bool, bool, uintptr_t> infiniteSun{false, false, 0};
    std::tuple<bool, bool, std::vector<uintptr_t>> infinitePlantHealth{false, false, {}}; // multi address 
    std::tuple<bool, bool, uintptr_t> instantActivatePotatoMine{false, false, 0};
    std::tuple<bool, bool, uintptr_t> instantPlantRecharge{false, false, 0};
    std::tuple<bool, bool, uintptr_t> noChomperCooldown{false, false, 0};
    std::tuple<bool, bool, uintptr_t> plantAnywhere{false, false, 0};
    std::tuple<bool, bool, uintptr_t> noZombies{false, false, 0};
    std::tuple<bool, bool, std::vector<uintptr_t>> oneHitKills{false, false, {}}; // multi address
} hacks;

class GameInfo
{
public:
    GameInfo(const wchar_t* gameName);
    ~GameInfo();

    [[nodiscard]] const HANDLE& getRefToHandle() const { return m_hPVZ; }
    [[nodiscard]] uintptr_t getModuleBaseAddress() const { return reinterpret_cast<uintptr_t>(m_mPVZ32.modBaseAddr); }

private:
    HANDLE m_hPVZ{};
    MODULEENTRY32 m_mPVZ32{};
    void populateHacks() const;
};
