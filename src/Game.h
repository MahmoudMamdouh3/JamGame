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
private:
    sf::RenderWindow m_window;
    sf::Clock m_clock;
    Player m_player;
    Map m_map;
    Renderer m_renderer;
    Menu m_menu;
    PauseMenu m_pauseMenu;
    GameState m_gameState;

public:
    Game();

    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void renderMenu();
    void renderGame();
};
