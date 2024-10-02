#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "resource.h"
#include <d3d11.h>

namespace gui
{
    // Data
    inline ID3D11Device* g_pd3dDevice{nullptr};
    inline ID3D11DeviceContext* g_pd3dDeviceContext{nullptr};
    inline IDXGISwapChain* g_pSwapChain{nullptr};
    inline bool g_SwapChainOccluded{false};
    inline UINT g_ResizeWidth{0};
    inline UINT g_ResizeHeight{0};
    inline ID3D11RenderTargetView* g_mainRenderTargetView{nullptr};

    // globals for window
    inline WNDCLASSEXW wc;
    inline HWND hwnd;
    inline constexpr int WINDOW_WIDTH = 540;
    inline constexpr int WINDOW_HEIGHT = 260;
    inline ImGuiIO* io{nullptr};


    // dx11
    bool CreateDeviceD3D();
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();
    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // win32 windows
    void CreateAppWindow(const wchar_t* windowName, const wchar_t* className);
    void DestroyAppWindow();

    // imgui
    void CreateImGui();
    void DestroyImGui();
    void BeginRender();
    void EndRender();
    void ApplyStyles();
}
