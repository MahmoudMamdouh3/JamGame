#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include "Config.h"

class Cube : public sf::Drawable
{
public:
    sf::ConvexShape top, left, right;

    Cube(sf::Vector2f pos, sf::Color color);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
