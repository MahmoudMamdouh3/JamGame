#include "Cube.h"
#include <cstdint> // Required for fixed width types like uint8_t

Cube::Cube(sf::Vector2f pos, sf::Color color)
{
    float hw = TILE_WIDTH / 2.f;
    float hh = TILE_HEIGHT / 2.f;
    float h = BLOCK_HEIGHT;

    // 1. Top Face
    top.setPointCount(4);
    top.setPoint(0, sf::Vector2f(0.f, 0.f));
    top.setPoint(1, sf::Vector2f(hw, hh));
    top.setPoint(2, sf::Vector2f(0.f, hh * 2.f));
    top.setPoint(3, sf::Vector2f(-hw, hh));
    top.setPosition(sf::Vector2f(pos.x, pos.y - h));
    top.setFillColor(color);

    // 2. Left Face
    left.setPointCount(4);
    left.setPoint(0, sf::Vector2f(-hw, hh));
    left.setPoint(1, sf::Vector2f(0.f, hh * 2.f));
    left.setPoint(2, sf::Vector2f(0.f, hh * 2.f + h));
    left.setPoint(3, sf::Vector2f(-hw, hh + h));
    left.setPosition(sf::Vector2f(pos.x, pos.y - h));

    sf::Color dark = color;
    dark.r = static_cast<std::uint8_t>(dark.r * 0.7f);
    dark.g = static_cast<std::uint8_t>(dark.g * 0.7f);
    dark.b = static_cast<std::uint8_t>(dark.b * 0.7f);
    left.setFillColor(dark);

    // 3. Right Face
    right.setPointCount(4);
    right.setPoint(0, sf::Vector2f(0.f, hh * 2.f));
    right.setPoint(1, sf::Vector2f(hw, hh));
    right.setPoint(2, sf::Vector2f(hw, hh + h));
    right.setPoint(3, sf::Vector2f(0.f, hh * 2.f + h));
    right.setPosition(sf::Vector2f(pos.x, pos.y - h));

    sf::Color darkest = color;
    darkest.r = static_cast<std::uint8_t>(darkest.r * 0.5f);
    darkest.g = static_cast<std::uint8_t>(darkest.g * 0.5f);
    darkest.b = static_cast<std::uint8_t>(darkest.b * 0.5f);
    right.setFillColor(darkest);
}

void Cube::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(left, states);
    target.draw(right, states);
    target.draw(top, states);
}