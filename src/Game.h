#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Map.h"
#include "Renderer.h"
#include "UI/Menu.h"
#include "UI/PauseMenu.h"
#include "Config.h"

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
    Menu m_menu;
    PauseMenu m_pauseMenu;
    GameState m_gameState;

    void processEvents();
    void update(float dt);
    void render();
    void renderMenu();
    void renderGame();
};