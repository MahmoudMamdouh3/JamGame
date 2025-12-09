#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Map.h"
#include "Renderer.h"
#include "UI\Menu.h"
#include "UI\PauseMenu.h"
#include "Config.h" // Needed for GameState
#include "AudioManager.h"

class Game
{
public:
    Game();
    void run();

private:
    sf::RenderWindow m_window;
    sf::Clock m_clock;

    // Core Game Systems
    Map m_map;
    Player m_player;
    Renderer m_renderer;

    // UI Systems
    Menu m_menu;
    PauseMenu m_pauseMenu;

    // State & Audio
    GameState m_gameState;
    AudioManager m_audio;

    void processEvents();
    void update(float dt);
    void render();
    void renderMenu();
    void renderGame();
};