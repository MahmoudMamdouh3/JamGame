#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"

class IsometricUtils
{
public:
    static sf::Vector2f gridToScreen(float x, float y)
    {
        return sf::Vector2f(
            (x - y) * (TILE_WIDTH / 2.0f),
            (x + y) * (TILE_HEIGHT / 2.0f));
    }
};
