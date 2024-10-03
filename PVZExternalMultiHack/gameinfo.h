#pragma once
#include "memory.h"
#include <tuple>

// <0> holds the "was checked" value for the menu
// <1> will determine if hack is enabled or enabled
// <2> holds the location in memory that stores the bytes we want to modify for the hack
// <3> holds our custom code which is referred to as newBytes
// <4> holds the original code which is referred to as oldBytes
// <5> is the length of those bytes (in bytes)
inline struct sHacks
{
    std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t> autoCollectItems{false, false, 0, nullptr, nullptr, 0};
    std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t> bypassSunLimit{false, false, 0, nullptr, nullptr, 0};
    std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t> fastSunProduction{false, false, 0, nullptr, nullptr, 0};
    std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t> instantHit{false, false, 0, nullptr, nullptr, 0};
    std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t> infiniteCoins{false, false, 0, nullptr, nullptr, 0};
    std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t> infiniteLawnMower{false, false, 0, nullptr, nullptr, 0};
    std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t> infiniteSun{false, false, 0, nullptr, nullptr, 0};
    std::tuple<bool, bool, std::vector<uintptr_t>, std::vector<const char*>, std::vector<const char*>, std::vector<size_t>> infinitePlantHealth{false, false, {}, {}, {}, {}}; // multi address 
    std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t> instantActivatePotatoMine{false, false, 0, nullptr, nullptr, 0};
    std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t> instantPlantRecharge{false, false, 0, nullptr, nullptr, 0};
    std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t> noChomperCooldown{false, false, 0, nullptr, nullptr, 0};
    std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t> plantAnywhere{false, false, 0, nullptr, nullptr, 0};
    std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t> noZombies{false, false, 0, nullptr, nullptr, 0};
    std::tuple<bool, bool, std::vector<uintptr_t>, std::vector<const char*>, std::vector<const char*>, std::vector<size_t>> oneHitKills{false, false, {}, {}, {}, {}}; // multi address
} hacks;

class GameInfo
{
public:
    explicit GameInfo(const wchar_t* gameName);
    ~GameInfo();

    [[nodiscard]] const HANDLE& getRefToHandle() const { return m_hPVZ; }
    [[nodiscard]] uintptr_t getModuleBaseAddress() const { return reinterpret_cast<uintptr_t>(m_mPVZ32.modBaseAddr); }
    [[nodiscard]] bool isGameRunning() const;

private:
    HANDLE m_hPVZ{};
    MODULEENTRY32 m_mPVZ32{};
    void populateHacks() const;
};
