#pragma once
#include <cstdint>

namespace GameEngine
{
    constexpr auto ScreenWidth = 1024;
    constexpr auto ScreenHeight = 768;

    // Backbuffer
    extern uint32_t BackBuffer[ScreenHeight][ScreenWidth];

    // Keyboard keys
    enum class KeyCode
    {
        Escape = 0x1B,
        Space = 0x20,
        Left = 0x25,
        Up = 0x26,
        Right = 0x27,
        Down = 0x28,
        Enter = 0x0D
    };

    // Input functions
    bool IsKeyPressed(KeyCode key);
    bool IsMouseButtonPressed(int mouseButtonIndex);

    // Cursor position
    int getCursorX();
    int getCursorY();

    // Window state
    bool IsWindowActive();

    // Buffer management
    void ClearBuffer();

    // Game lifecycle
    const char* gameName();
    void initialize();
    void update(float deltaTime);
    void render();
    void shutdown();
    void quit();
} // namespace GameEngine