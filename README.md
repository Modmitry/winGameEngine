# Simple C++ Game Engine

This project is a basic C++ game engine built using Windows API (Win32). It is designed for creating simple console-based games, providing fundamental features such as:

- **Rendering**: Handles backbuffer and screen updates.
- **Input Handling**: Detects keyboard and mouse inputs.
- **Window Management**: Initializes and manages the game window.
- **Game Loop**: Handles updates and draws at each frame.

## Features

- Handle basic input from keyboard and mouse.
- Rendering system using Win32 API for pixel-level control.
- Adjustable screen resolution for different projects.
- Simple game loop that updates game logic and renders frames.

## Requirements

- Microsoft Visual Studio 2022 (or later)
- Windows operating system (tested on Windows 10)

## Getting Started



## How to create a Simple Game
To create your own game, you need to override a few methods in the WinGame.cpp file. These methods allow you to define your game's name, initialization logic, game update loop, rendering logic, and shutdown sequence.

Summary of Methods to Override:
const char* gameName(): Set the game window title.
void initialize(): Initialize game data.
void update(float dt): Update game logic every frame.
void render(): Render the game each frame.
void shutdown(): Clean up game resources after quitting.
