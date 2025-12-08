#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include "Config.h"

class Renderer {
private:
    sf::RenderWindow& m_window;
    sf::Vector2f m_cameraOffset;

    void renderTile(const Map& map, int x, int y);
    void renderPlayer(const Player& player, const Map& map, int x, int y);

public:
    Renderer(sf::RenderWindow& window);

    void render(const Map& map, const Player& player);
};