#pragma once
#include <SFML/Graphics.hpp>

class IsometricUtils {
public:
    // Define tile size constants here so they are shared
    static constexpr float TILE_WIDTH = 64.0f;
    static constexpr float TILE_HEIGHT = 32.0f;

    // Just the declarations
    static sf::Vector2f gridToScreen(float x, float y);
    static sf::Vector2f screenToGrid(sf::Vector2f screenPos);
};