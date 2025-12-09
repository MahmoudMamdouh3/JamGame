#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include "Follower.h"
#include "Config.h"

class Renderer
{
public:
    Renderer(sf::RenderWindow &window);

    void update(float dt, sf::Vector2f playerGridPos);
    void render(const Map &map, const Player &player);

    // New function to toggle grid lines
    void toggleGrid();

private:
    sf::RenderWindow &m_window;

    // The current shift applied to the world
    sf::Vector2f m_cameraOffset;
    bool m_showGrid; // State for 'T' key

    // Helper for smooth movement (Linear Interpolation)
    sf::Vector2f lerp(const sf::Vector2f &start, const sf::Vector2f &end, float t);

    void renderTile(const Map &map, int x, int y);
    void renderPlayer(const Player &player, const Map &map, int x, int y);
    void renderFollower(const Follower &follower, const Map &map, int x, int y);
};