#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <iostream>
#include <Windows.h>
#include <dwmapi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <tchar.h>
#include <vector>
#include <TlHelp32.h>
#include <thread>
#include "colors.h"
#include "WindowThread.h"
#include "font_buffers.h"
#include "Line.h"
#include "Circle.h"
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "binding.h"
#include "Macro.h"
#include "col.h"

float random_float(float min, float max) {
    float scale = rand() / (float)RAND_MAX;
    return min + scale * (max - min);
}

static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;
HWND                            hwnd = NULL;
bool                            drawMenu = true;
int                             alpha = 0;
static int                      backgroundAlpha = 100;
static std::vector<Circle*>     circles;
static std::vector<Line*>       lines;
static short                    menuKey = VK_INSERT;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main(int, char**)
{
    FreeConsole();
    std::string log = "[ Start Of Log - Freed Console ]";
    WNDCLASSEX wc;

    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = _T("Aero\'s Macro");
    wc.lpszMenuName = nullptr;
    wc.style = CS_VREDRAW | CS_HREDRAW;

    ::RegisterClassEx(&wc);
    const HWND hwnd = ::CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, wc.lpszClassName, _T("0verlay"), WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, wc.hInstance, nullptr);

    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
    const MARGINS margin = { -1, 0, 0, 0 };
    DwmExtendFrameIntoClientArea(hwnd, &margin);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    log += "\n[ Direct3D11 Initialized ]";

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    io.Fonts->AddFontFromMemoryTTF(
        &stolzl_medium_bin,
        sizeof(stolzl_medium_bin),
        16.f
    );

    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    io.Fonts->AddFontFromMemoryTTF(font_awesome_bin, sizeof(font_awesome_bin), 11.f, &icons_config, icon_ranges);

    log += "\n[ Fonts Loaded ]";

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    log += "\n[ ImGui Initialized ]";

    // start thread
    WindowThread* windowThread = new WindowThread();
    std::thread thread(&WindowThread::update, windowThread);

    log += "\n[ Window Listener Thread Started ]";

    Macro* macroThread = new Macro();
    std::thread macro(&Macro::loop, macroThread);

    log += "\n[ Macro Listener Thread Started ]";

    log += "\n[ Roblox @ 0x";
    log += std::to_string((unsigned int)windowThread->hwnd);
    log += " ]";

    bool done = false;

    SetActiveWindow(windowThread->hwnd);
    SetActiveWindow(hwnd);

    srand(time(0));


    for (int i = 0; i < 100; i++)
    {
        Circle* circle = new Circle();
        circle->centerX = rand() % (int)windowThread->windowSize.x;
        circle->centerY = rand() % (int)windowThread->windowSize.x;
        circle->dx = random_float(-50, 50);
        circle->dy = random_float(-50, 50);

        while (circle->dy == circle->dx)
        {
            circle->dy = random_float(-1.5, 1.5);
        }

        circles.push_back(circle);
    }

    while (!done)
    {

        

        Colors::style();
        SetWindowPos(hwnd, HWND_TOPMOST, windowThread->windowPos.x, windowThread->windowPos.y, windowThread->windowSize.x, windowThread->windowSize.y, SWP_SHOWWINDOW);
        UpdateWindow(hwnd);

        if (GetAsyncKeyState(menuKey) & 1)
        {
            drawMenu = !drawMenu;

            log += "\n[ Menu Toggled ]";

            if (drawMenu)
            {
                SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
            }
            else {
                for (Circle* circle : circles)
                {
                    circle->centerX = rand() % (int)windowThread->windowSize.x;
                    circle->centerY = rand() % (int)windowThread->windowSize.x;
                    circle->dx = random_float(-50, 50);
                    circle->dy = random_float(-50, 50);

                    while (circle->dy == circle->dx)
                    {
                        circle->dy = random_float(-1.5, 1.5);
                    }
                }
                SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
            }
        }

        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();



        if (drawMenu)
        {

            if (config::particles)
            {
                for (Circle* c : circles) { c->render(); }


                for (Circle* c : circles)
                {
                    
                    if (config::move)
                    {
                        c->centerX += c->dx / config::particleInterpolation;
                        c->centerY += c->dy / config::particleInterpolation;

                        if (c->centerX <= 5 || c->centerY <= 5 || c->centerX >= windowThread->windowSize.x || c->centerY >= windowThread->windowSize.y)
                        {
                            c->dx = -c->dx;
                            c->dy = -c->dy;
                            c->centerX = rand() % (int)windowThread->windowSize.x;
                            c->centerY = rand() % (int)windowThread->windowSize.x;
                        }
                    }
                    

                    for (Circle* c2 : circles)
                    {
                        if (c == c2)
                            continue;

                        if (c->distance(c2) > 100)
                            continue;

                        Line* line = new Line();
                        line->x1 = c->centerX;
                        line->x2 = c2->centerX;

                        line->y1 = c->centerY;
                        line->y2 = c2->centerY;
                        line->render();
                    }
                }
            }
            
            // Rectangle Background
            ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(windowThread->windowSize.x, windowThread->windowSize.y), ImColor(0, 0, 0, backgroundAlpha));
            lines.clear();
            
            // Render particles nd shit
            


            ImGui::SetNextWindowSize(ImVec2(300, 50));
            ImGui::SetNextWindowPos(ImVec2((windowThread->windowSize.x / 2) - 150, 25));

            ImGuiStyle& style = ImGui::GetStyle();


            style.WindowBorderSize = 0.0f;
            style.WindowRounding = 12.5f;
            style.FrameRounding = 8.f;

            style.ButtonTextAlign = ImVec2(0.5, 0.5);
            style.FramePadding = ImVec2(8, 8);

            ImGui::Begin("menuBar", nullptr, ImGuiWindowFlags_NoDecoration);

            float y = ImGui::GetCursorPosY();
            float x = ImGui::GetCursorPosX();

            ImGui::SetCursorPos(ImVec2(x + 25, y));
            if (ImGui::Button(ICON_FA_GAMEPAD))
            {
                Colors::current_tab = tab::main_page;
                alpha = 255;
            }

            ImGui::SetCursorPos(ImVec2(x + 75, y));
            x += 50;
            if (ImGui::Button(ICON_FA_EYE))
            {
                Colors::current_tab = tab::visuals;
                alpha = 255;
            }

            ImGui::SetCursorPos(ImVec2(x + 75, y));
            x += 50;
            if (ImGui::Button(ICON_FA_GEAR))
            {
                Colors::current_tab = tab::macrosettings;
                alpha = 255;
            }

            ImGui::SetCursorPos(ImVec2(x + 75, y));
            x += 50;
            if (ImGui::Button(ICON_FA_PALETTE))
            {
                Colors::current_tab = tab::colors;
                alpha = 255;
            }

            ImGui::SetCursorPos(ImVec2(x + 75, y));
            x += 50;
            if (ImGui::Button(ICON_FA_SKULL))
            {
                exit(0);
            }


            ImGui::End();
            Colors::style();

            {
                ImGui::SetNextWindowSize(ImVec2(800, 400));
                ImGui::Begin("Horizon V2 Concept", nullptr, ImGuiWindowFlags_NoDecoration);

                switch (Colors::current_tab)
                {
                    case tab::main_page:
                    {
                        ImGui::Text("Main Macro Page");
                        ImGui::SliderInt("First Person Delay", &macroThread->first_person_delay_ms, 1, 100);
                        ImGui::SliderInt("Third Person Delay", &macroThread->third_person_delay_ms, 1, 100);
                        ImGui::Text("You can find more settings in the Macro Settings tab!");
                        ImGui::Separator();

                        ImGui::Text("If the key binding does not work, re-toggle the menu!");
                        ImGui::HotKey("First Person Key", macroThread->first_person_key, ImVec2(100, 20));
                        ImGui::HotKey("Third Person Key", macroThread->third_person_key, ImVec2(100, 20));

                        ImGui::Separator();
                        ImGui::HotKey("Menu Key", menuKey, ImVec2(100, 20));

                        break;
                    }

                    case tab::visuals:
                    {
                        ImGui::Text("Visuals");
                        ImGui::SliderInt("Backgound Alpha", &backgroundAlpha, 0, 255);
                        ImGui::Checkbox("Particles", &config::particles);
                        ImGui::SliderFloat("Particle Interpolation", &config::particleInterpolation, 1.f, 750.f);
                        ImGui::Checkbox("Move Particles", &config::move);

                        ImGui::Separator();
                        ImGui::Text("Window Info | { %f, %f. %f. %f }", windowThread->rect.left, windowThread->rect.top, windowThread->rect.right, windowThread->rect.left);
                        ImGui::Text("Mouse Vector: { %f, %f }", ImGui::GetMousePos().x, ImGui::GetMousePos().y);

                        break;
                    }

                    case tab::macrosettings:
                    {
                        ImGui::Checkbox("Output", &config::output);
                        ImGui::Checkbox("First Person Macro (IO)", &macroThread->first_person);
                        ImGui::Checkbox("Third Person macro (SCROLL WHEEL)", &macroThread->third_person);

                        ImGui::Text("The macro is now hold only, toggling is broken.");
                        //ImGui::Checkbox("Toggle (Press instead of hold)", &macroThread->toggle);

                        ImGui::Checkbox("Sleep Twice", &macroThread->sleep_twice);

                        if (config::output)
                        {
                            ImGui::BeginChild("LogOutput");
                            ImGui::Text(log.c_str()); ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(1000, 1000), ImColor(7, 8, 13, alpha));
                            ImGui::EndChild();
                        }

                        break;
                    }

                    case tab::colors:
                    {
                        ImGui::Text("Colors");

                        ImGui::ColorEdit3("Circles Color", RenderColors::circleColor);
                        ImGui::ColorEdit3("Lines Color", RenderColors::lineColor);

                        break;
                    }

                    default:
                    {
                        break;
                    }
                }

                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(1000, 1000), ImColor(7, 8, 13, alpha));

                ImGui::End();

                
            }
        }

        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm* localTime = std::localtime(&currentTime);

        if (!localTime)
        {
            log += "\n[ Error Getting Local Time ]";
        }
        else {
            int hours24 = localTime->tm_hour;
            bool isPM = (hours24 >= 12);
            int hours12 = (hours24 % 12) == 0 ? 12 : (hours24 % 12);

            std::string text = "Astcro [ Stable Build ] [" + std::to_string((int)ImGui::GetIO().Framerate) + "] [ " + std::to_string(hours12) + ":" + std::to_string(localTime->tm_min) + ":" + std::to_string(localTime->tm_sec) + " " + (isPM ? "PM" : "AM") + " ]";

            ImGui::GetForegroundDrawList()->AddText(ImVec2(10, 10), ImColor(255, 255, 255, 255), text.c_str());
        }

        ImGui::Render(); if (alpha > 5) alpha -= 5;
        const float clear_color_with_alpha[4] = { 0, 0, 0, 0 };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        if (config::vsync)
        {
            g_pSwapChain->Present(1, 0);
        }
        else {
            g_pSwapChain->Present(0, 0);
        }
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
