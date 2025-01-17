#define WIN32_LEAN_AND_MEAN
#include "WinEngine.h"
#include <windows.h>
#include <cstring>

namespace GameEngine
{
    uint32_t BackBuffer[ScreenHeight][ScreenWidth] = {};

    static HINSTANCE InstanceHandle = nullptr;
    static bool is_active = true;
    static POINT CursorPosition;
    static bool quit_flag = false;
    static LARGE_INTEGER qpc_frequency = {};
    static LARGE_INTEGER qpc_reference_time = {};

    // Check if the window is active
    bool IsWindowActive()
    {
        return is_active;
    }

    // Clear the backbuffer
    void ClearBuffer()
    {
        std::memset(BackBuffer, 0, sizeof(BackBuffer));
    }

    // Check if a key is pressed
    bool IsKeyPressed(KeyCode key)
    {
        return is_active && (GetAsyncKeyState(static_cast<int>(key)) & 0x8000);
    }

    // Check if a mouse button is pressed
    bool IsMouseButtonPressed(int mouseButtonIndex)
    {
        if (!is_active)
            return false;

        return mouseButtonIndex == 0 ? GetAsyncKeyState(VK_LBUTTON) != 0
            : GetAsyncKeyState(VK_RBUTTON) != 0;
    }

    // Get cursor X position
    int getCursorX()
    {
        return CursorPosition.x;
    }

    // Get cursor Y position
    int getCursorY()
    {
        return CursorPosition.y;
    }

    // Signal to quit the game
    void quit()
    {
        quit_flag = true;
    }

    // Game loop update procedure
    static void CALLBACK UpdateProcedure(HWND hwnd)
    {
        if (quit_flag)
            return;

        is_active = GetActiveWindow() == hwnd;

        GetCursorPos(&CursorPosition);
        ScreenToClient(hwnd, &CursorPosition);

        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);

        float deltaTime = static_cast<float>(currentTime.QuadPart - qpc_reference_time.QuadPart) / qpc_frequency.QuadPart;
        if (deltaTime > 0.1f)
            deltaTime = 0.1f;

        update(deltaTime);

        if (!quit_flag)
        {
            render();
            RedrawWindow(hwnd, nullptr, 0, RDW_INVALIDATE | RDW_UPDATENOW);
        }

        qpc_reference_time = currentTime;
    }

    // Window procedure
    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            BITMAPINFOHEADER bih = {};
            bih.biSize = sizeof(bih);
            bih.biWidth = ScreenWidth;
            bih.biHeight = -ScreenHeight;
            bih.biPlanes = 1;
            bih.biBitCount = 32;
            bih.biCompression = BI_RGB;

            SetDIBitsToDevice(
                hdc,
                0, 0,
                ScreenWidth, ScreenHeight,
                0, 0,
                0, ScreenHeight,
                BackBuffer,
                reinterpret_cast<BITMAPINFO*>(&bih),
                DIB_RGB_COLORS);

            EndPaint(hwnd, &ps);
        }
        break;

        case WM_DESTROY:
            quit_flag = true;
            break;

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        return 0;
    }

    // Entry point
    static int mainEntry(HINSTANCE hInstance, int nCmdShow)
    {
        InstanceHandle = hInstance;

        WNDCLASSEXA windowClass = {};
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = WindowProcedure;
        windowClass.hInstance = hInstance;
        windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        windowClass.lpszClassName = "GameTemplateWndClass";

        RegisterClassExA(&windowClass);

        RECT windowRect = { 0, 0, ScreenWidth, ScreenHeight };
        AdjustWindowRectEx(&windowRect, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, FALSE, 0);

        HWND hwnd = CreateWindowA(
            windowClass.lpszClassName, gameName(), WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
            CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
            nullptr, nullptr, hInstance, nullptr);

        if (!hwnd)
            return 0;

        ShowWindow(hwnd, nCmdShow);
        UpdateWindow(hwnd);

        QueryPerformanceFrequency(&qpc_frequency);
        QueryPerformanceCounter(&qpc_reference_time);

        initialize();

        MSG msg = {};
        while (!quit_flag)
        {
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            UpdateProcedure(hwnd);
            Sleep(0);
        }

        shutdown();

        return static_cast<int>(msg.wParam);
    }
} // namespace GameEngine

// Provide a globally accessible `wWinMain` for the linker
extern "C" int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    return GameEngine::mainEntry(hInstance, nCmdShow);
}