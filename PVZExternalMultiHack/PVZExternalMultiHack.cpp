#include <vector>
#include <iostream>
#include <Windows.h>

#include "gameinfo.h"
#include "gui.h"
#include "memory.h"
#include "imgui/imgui_internal.h"

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR arguments, int commandShow) try
{
    // create window for hack gui
    gui::CreateAppWindow(L"PVZ Multihack", L"PVZ Multihack Class");
    // initialize d3d
    gui::CreateDeviceD3D();
    // create imgui context
    gui::CreateImGui();
    // apply styles
    gui::ApplyStyles();

    // instantiate GameInfo class for PVZ
    const GameInfo infoPVZ(L"popcapgame1.exe");

    // instantiate hook info structs for hacks that require custom code to run
    sHookInfo hiFastSunProduction(std::get<3>(hacks.fastSunProduction), std::get<5>(hacks.fastSunProduction),
                                  "\xC7\x47\x58\x01\x00\x00\x00\xFF\x4F\x58\x8B\x77\x58\xE9\xFF\xFF\xFF\xFF", 18);
    sHookInfo hiInfiniteCoins(std::get<3>(hacks.infiniteCoins), std::get<5>(hacks.infiniteCoins),
                              "\xC7\x41\x54\x9F\x86\x01\x00\x8B\x51\x54\x52\x8D\x44\x24\x30\xE9\xFF\xFF\xFF\xFF", 20);
    sHookInfo hiInfiniteSun(std::get<3>(hacks.infiniteSun), std::get<5>(hacks.infiniteSun),
                            "\xC7\x87\x78\x55\x00\x00\x0F\x27\x00\x00\x8B\x87\x78\x55\x00\x00\xE9\xFF\xFF\xFF\xFF", 21);
    sHookInfo hiInstantPlantRecharge(std::get<3>(hacks.instantPlantRecharge), std::get<5>(hacks.instantPlantRecharge),
                                     "\x81\x47\x24\x00\x02\x00\x00\x8B\x47\x24\xE9\xFF\xFF\xFF\xFF", 15);
    sHookInfo hiNoZombies(std::get<3>(hacks.noZombies), std::get<5>(hacks.noZombies),
                          "\xB8\x00\x00\x00\x00\xC1\xE0\x10\xE9\xFF\xFF\xFF\xFF", 13);

    // set loop condition
    bool running{true};
    // start hack loop
    while (running)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        // if window gets closed then break out of loop
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                running = false;
        }
        if (!infoPVZ.isGameRunning())
            break;

        if (!running)
            disableAllHacks();

        // skeleton for hacks
        if (std::get<0>(hacks.autoCollectItems))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.autoCollectItems);
        }
        if (std::get<0>(hacks.bypassSunLimit))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.bypassSunLimit);
        }
        if (std::get<0>(hacks.fastSunProduction))
        {
            toggleHackWithHook(infoPVZ.getRefToHandle(), hacks.fastSunProduction, hiFastSunProduction);
        }
        if (std::get<0>(hacks.instantHit))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.instantHit);
        }
        if (std::get<0>(hacks.infiniteCoins))
        {
            toggleHackWithHook(infoPVZ.getRefToHandle(), hacks.infiniteCoins, hiInfiniteCoins);
        }
        if (std::get<0>(hacks.infiniteLawnMower))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.infiniteLawnMower);
        }
        if (std::get<0>(hacks.infiniteSun))
        {
            toggleHackWithHook(infoPVZ.getRefToHandle(), hacks.infiniteSun, hiInfiniteSun);
        }
        if (std::get<0>(hacks.infinitePlantHealth))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.infinitePlantHealth);
        }
        if (std::get<0>(hacks.instantActivatePotatoMine))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.instantActivatePotatoMine);
        }
        if (std::get<0>(hacks.instantPlantRecharge))
        {
            toggleHackWithHook(infoPVZ.getRefToHandle(), hacks.instantPlantRecharge, hiInstantPlantRecharge);
        }
        if (std::get<0>(hacks.noChomperCooldown))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.noChomperCooldown);
        }
        if (std::get<0>(hacks.plantAnywhere))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.plantAnywhere);
        }
        if (std::get<0>(hacks.noZombies))
        {
            toggleHackWithHook(infoPVZ.getRefToHandle(), hacks.noZombies, hiNoZombies);
        }
        if (std::get<0>(hacks.oneHitKills))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.oneHitKills);
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;

        // imgui window initialization
        gui::BeginRender();

        // window stuff
        ImGui::SetNextWindowPos({0, 0});
        ImGui::SetNextWindowSize({gui::WINDOW_WIDTH, gui::WINDOW_HEIGHT});
        ImGui::Begin("PVZ Multi Hack", nullptr,
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoTitleBar);
        // create left side of window
        ImGui::BeginChild("##left_side",
                          ImVec2(ImGui::GetContentRegionAvail().x / 2.0f, ImGui::GetContentRegionAvail().y));
        if (ImGui::Checkbox("Auto Collect Items", &std::get<1>(hacks.autoCollectItems)))
            std::get<0>(hacks.autoCollectItems) = true;
        if (ImGui::Checkbox("Bypass Sun Limit", &std::get<1>(hacks.bypassSunLimit)))
            std::get<0>(hacks.bypassSunLimit) = true;
        if (ImGui::Checkbox("Fast Sun Production", &std::get<1>(hacks.fastSunProduction)))
            std::get<0>(hacks.fastSunProduction) = true;
        if (ImGui::Checkbox("Instant Hit", &std::get<1>(hacks.instantHit))) std::get<0>(hacks.instantHit) = true;
        if (ImGui::Checkbox("Infinite Coins", &std::get<1>(hacks.infiniteCoins)))
            std::get<0>(hacks.infiniteCoins) = true;
        if (ImGui::Checkbox("Infinite Lawn Mower", &std::get<1>(hacks.infiniteLawnMower)))
            std::get<0>(hacks.infiniteLawnMower) = true;
        if (ImGui::Checkbox("Infinite Sun", &std::get<1>(hacks.infiniteSun))) std::get<0>(hacks.infiniteSun) = true;
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::SameLine();

        ImGui::BeginChild("##right_side", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
        if (ImGui::Checkbox("Infinite Plant Health", &std::get<1>(hacks.infinitePlantHealth)))
            std::get<0>(hacks.infinitePlantHealth) = true;
        if (ImGui::Checkbox("Instant Activate Potato Mine", &std::get<1>(hacks.instantActivatePotatoMine)))
            std::get<0>(hacks.instantActivatePotatoMine) = true;
        if (ImGui::Checkbox("Instant Plant Recharge", &std::get<1>(hacks.instantPlantRecharge)))
            std::get<0>(hacks.instantPlantRecharge) = true;
        if (ImGui::Checkbox("No Chomper Cooldown", &std::get<1>(hacks.noChomperCooldown)))
            std::get<0>(hacks.noChomperCooldown) = true;
        if (ImGui::Checkbox("No Zombies", &std::get<1>(hacks.noZombies)))
            std::get<0>(hacks.noZombies) = true;
        if (ImGui::Checkbox("One Hit Kills", &std::get<1>(hacks.oneHitKills)))
            std::get<0>(hacks.oneHitKills) = true;
        if (ImGui::Checkbox("Plant Anywhere", &std::get<1>(hacks.plantAnywhere)))
            std::get<0>(hacks.plantAnywhere) = true;
        ImGui::EndChild();
        ImGui::End();

        // imgui window cleanup
        gui::EndRender();
        if (!running) break;
        Sleep(10);
    }

    // cleanup
    gui::DestroyImGui();
    gui::CleanupDeviceD3D();
    gui::DestroyAppWindow();

    return EXIT_SUCCESS;
}
catch (std::runtime_error& e)
{
    MessageBoxA(0, e.what(), "ERROR", 0);
    return EXIT_FAILURE;
}
