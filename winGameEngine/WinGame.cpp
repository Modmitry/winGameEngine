#include "WinEngine.h"
#include <cstring>

namespace GameEngine
{
    const char* gameName()
    {
        // Function to return the game name
	    return "GameEngine";
    }

    void initialize()
    {
        // Initialize game resources
    }

    void update(float deltaTime)
    {
        if (IsKeyPressed(KeyCode::Escape))
            quit();

        // Update game state
    }

    void render()
    {
        ClearBuffer();
        // Draw game objects to the buffer
    }

    void shutdown()
    {
        // Clean up game resources
    }
} // namespace GameEngine
