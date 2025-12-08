#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include "Config.h"

class Renderer {
private:
    sf::RenderWindow& m_window;

    // The current shift applied to the world
    sf::Vector2f m_cameraOffset;

    // Helper for smooth movement (Linear Interpolation)
    sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t);

    void renderTile(const Map& map, int x, int y);
    void renderPlayer(const Player& player, const Map& map, int x, int y);

public:
    Renderer(sf::RenderWindow& window);

    // New: Updates the camera position to follow the target
    void update(float dt, sf::Vector2f targetGridPos);

    void render(const Map& map, const Player& player);
};