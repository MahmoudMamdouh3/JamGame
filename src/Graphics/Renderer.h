#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include "Config.h" // Ensure WINDOW_WIDTH/HEIGHT are here

class Renderer {
public:
    Renderer(sf::RenderWindow& window);

    void update(float dt, sf::Vector2f playerGridPos);
    void render(const Map& map, const Player& player);

    // New function to toggle grid lines
    void toggleGrid();

private:
    sf::RenderWindow& m_window;
    sf::Vector2f m_cameraOffset;
    bool m_showGrid; // State for 'T' key

    sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t);
    void renderTile(const Map& map, int x, int y);
    void renderPlayer(const Player& player, const Map& map, int x, int y);
};