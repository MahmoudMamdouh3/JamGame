#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"

class Player
{
private:
    sf::Vector2f m_position;
    float m_z;
    float m_velocityZ;
    sf::CircleShape m_shape;
    sf::CircleShape m_shadow;

public:
    Player();

    void handleInput(float dt, int heights[MAP_SIZE][MAP_SIZE]);
    void update(float dt, int heights[MAP_SIZE][MAP_SIZE]);
    void jump(int heights[MAP_SIZE][MAP_SIZE]);

    sf::Vector2f getPosition() const { return m_position; }
    float getZ() const { return m_z; }
    const sf::CircleShape &getShape() const { return m_shape; }
    const sf::CircleShape &getShadow() const { return m_shadow; }
    void setPosition(sf::Vector2f pos) { m_position = pos; }
};
