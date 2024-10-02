#include <vector>
#include <iostream>
#include <Windows.h>

#include "gameinfo.h"
#include "gui.h"
#include "memory.h"
#include "imgui/imgui_internal.h"

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR arguments, int commandShow)
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
        if (!running)
            break;

        // skeleton for hacks
        if (std::get<0>(hacks.autoCollectItems))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.autoCollectItems, "Auto collect items", "\xeb", "\x75", 1);
        }
        if (std::get<0>(hacks.bypassSunLimit))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.bypassSunLimit, "Bypass sun limit", "\xeb", "\x7e", 1);
        }
        if (std::get<0>(hacks.fastSunProduction))
        {
            toggleHackWithHook(infoPVZ.getRefToHandle(), hacks.fastSunProduction, hiFastSunProduction,
                               "Fast sun production",
                               "\xFF\x4F\x58\x8B\x77\x58", 6);
        }
        if (std::get<0>(hacks.instantHit))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.instantHit, "Instant hit", "\x90\x90", "\x7c\x1b", 2);
        }
        if (std::get<0>(hacks.infiniteCoins))
        {
            toggleHackWithHook(infoPVZ.getRefToHandle(), hacks.infiniteCoins, hiInfiniteCoins, "Infinite coins",
                               "\x8B\x51\x54\x52\x8D\x44\x24\x30", 8);
        }
        if (std::get<0>(hacks.infiniteLawnMower))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.infiniteLawnMower, "Infinite lawn mower",
                       "\x90\x90\x90\x90\x90\x90\x90", "\xc7\x46\x2c\x02\x00\x00\x00", 7);
        }
        if (std::get<0>(hacks.infiniteSun))
        {
            toggleHackWithHook(infoPVZ.getRefToHandle(), hacks.infiniteSun, hiInfiniteSun, "Infinite sun",
                               "\x8B\x87\x78\x55\x00\x00", 6);
        }
        if (std::get<0>(hacks.infinitePlantHealth))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.infinitePlantHealth, "Infinite plant health",
                       {"\x90\x90\x90", "\x90\x90\x90\x90"}, {"\x29\x50\x40", "\x83\x46\x40\xFC"}, {3, 4});
        }
        if (std::get<0>(hacks.instantActivatePotatoMine))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.instantActivatePotatoMine, "Instant activate potato mine",
                       "\x90\x90\x90\x90\x90\x90",
                       "\x0f\x85\xfd\x01\x00\x00", 6);
        }
        if (std::get<0>(hacks.instantPlantRecharge))
        {
            toggleHackWithHook(infoPVZ.getRefToHandle(), hacks.instantPlantRecharge, hiInstantPlantRecharge,
                               "Instant plant recharge", "\xFF\x47\x24\x8B\x47\x24", 6);
        }
        if (std::get<0>(hacks.noChomperCooldown))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.noChomperCooldown, "No chomper cooldown", "\x90\x90", "\x75\x5f",
                       2);
        }
        if (std::get<0>(hacks.plantAnywhere))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.plantAnywhere, "Plant anywhere", "\x0F\x84",
                       "\x0F\x85", 2);
        }
        if (std::get<0>(hacks.noZombies))
        {
            toggleHackWithHook(infoPVZ.getRefToHandle(), hacks.noZombies, hiNoZombies, "No zombies",
                               "\x8B\x46\x14\xC1\xE0\x10", 6);
        }
        if (std::get<0>(hacks.oneHitKills))
        {
            toggleHack(infoPVZ.getRefToHandle(), hacks.oneHitKills, "One hit kills",
                       {"\xBD\x00\x00\x00\x00\x90", "\xB9\x00\x00\x00\x00\x90", "\x90\x90\x90\x90\x90\x90"}, {
                           "\x8B\xAF\xC8\x00\x00\x00", "\x8B\x8D\xD0\x00\x00\x00", "\x29\x86\xDC\x00\x00\x00"
                       }, {6, 6, 6});
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
        if (ImGui::Checkbox("Auto Collect Items", &std::get<1>(hacks.autoCollectItems))) std::get<0>(
            hacks.autoCollectItems) = true;
        if (ImGui::Checkbox("Bypass Sun Limit", &std::get<1>(hacks.bypassSunLimit))) std::get<0>(hacks.bypassSunLimit) =
            true;
        if (ImGui::Checkbox("Fast Sun Production", &std::get<1>(hacks.fastSunProduction))) std::get<0>(
            hacks.fastSunProduction) = true;
        if (ImGui::Checkbox("Instant Hit", &std::get<1>(hacks.instantHit))) std::get<0>(hacks.instantHit) = true;
        if (ImGui::Checkbox("Infinite Coins", &std::get<1>(hacks.infiniteCoins))) std::get<0>(hacks.infiniteCoins) =
            true;
        if (ImGui::Checkbox("Infinite Lawn Mower", &std::get<1>(hacks.infiniteLawnMower))) std::get<0>(
            hacks.infiniteLawnMower) = true;
        if (ImGui::Checkbox("Infinite Sun", &std::get<1>(hacks.infiniteSun))) std::get<0>(hacks.infiniteSun) = true;
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::SameLine();

        ImGui::BeginChild("##right_side", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
        if (ImGui::Checkbox("Infinite Plant Health", &std::get<1>(hacks.infinitePlantHealth))) std::get<0>(
            hacks.infinitePlantHealth) = true;
        if (ImGui::Checkbox("Instant Activate Potato Mine", &std::get<1>(hacks.instantActivatePotatoMine))) std::get<0>(
            hacks.instantActivatePotatoMine) = true;
        if (ImGui::Checkbox("Instant Plant Recharge", &std::get<1>(hacks.instantPlantRecharge))) std::get<0>(
            hacks.instantPlantRecharge) = true;
        if (ImGui::Checkbox("No Chomper Cooldown", &std::get<1>(hacks.noChomperCooldown))) std::get<0>(
            hacks.noChomperCooldown) = true;
        if (ImGui::Checkbox("No Zombies", &std::get<1>(hacks.noZombies))) std::get<0>(hacks.noZombies) = true;
        if (ImGui::Checkbox("One Hit Kills", &std::get<1>(hacks.oneHitKills))) std::get<0>(hacks.oneHitKills) = true;
        if (ImGui::Checkbox("Plant Anywhere", &std::get<1>(hacks.plantAnywhere))) std::get<0>(hacks.plantAnywhere) =
            true;
        ImGui::EndChild();
        ImGui::End();

        // imgui window cleanup
        gui::EndRender();
        Sleep(10);
    }

    // cleanup
    gui::DestroyImGui();
    gui::CleanupDeviceD3D();
    gui::DestroyAppWindow();

    return 0;
}
