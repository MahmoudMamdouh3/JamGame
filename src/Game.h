#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Map.h"
#include "Renderer.h"
#include "Config.h"

class Game {
private:
    sf::RenderWindow m_window;
    sf::Clock m_clock;

    // Core Game Systems
    Map m_map;
    Player m_player;
    Renderer m_renderer;

    void processEvents();
    void update(float dt);
    void render();

public:
    Game();
    void run();
};