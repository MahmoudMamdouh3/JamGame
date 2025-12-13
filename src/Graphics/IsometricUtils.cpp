#include "IsometricUtils.h"
#include <cmath>

// 1. Grid to Screen
sf::Vector2f IsometricUtils::gridToScreen(float x, float y)
{
    float screenX = (x - y) * (TILE_WIDTH / 2.0f);
    float screenY = (x + y) * (TILE_HEIGHT / 2.0f);
    return sf::Vector2f(screenX, screenY);
}

// 2. Screen to Grid
sf::Vector2f IsometricUtils::screenToGrid(sf::Vector2f screenPos)
{
    float halfW = TILE_WIDTH / 2.0f;
    float halfH = TILE_HEIGHT / 2.0f;

    // Reverse the isometric projection math
    float x = (screenPos.y / halfH + screenPos.x / halfW) / 2.0f;
    float y = (screenPos.y / halfH - screenPos.x / halfW) / 2.0f;

    return sf::Vector2f(x, y);
}