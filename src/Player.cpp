#include "Player.h"
#include <SFML/Graphics.hpp>
#include <cmath>

Player::Player()
    : m_position(MAP_SIZE / 2.0f, MAP_SIZE / 2.0f), m_z(0.0f), m_velocityZ(0.0f)
{
    // Initialize Player Visuals
    m_shape.setRadius(15.f);
    m_shape.setOrigin(sf::Vector2f(15.f, 30.f));
    m_shape.setFillColor(sf::Color::Cyan);

    m_shadow.setRadius(15.f);
    m_shadow.setOrigin(sf::Vector2f(15.f, 15.f));
    m_shadow.setScale(sf::Vector2f(1.f, 0.5f));
    m_shadow.setFillColor(sf::Color(0, 0, 0, 100));
}

void Player::handleInput(float dt, int heights[MAP_SIZE][MAP_SIZE])
{
    sf::Vector2f move(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        move.y -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        move.y += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        move.x -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        move.x += 1;

    if (move.x != 0 || move.y != 0)
    {
        float len = std::sqrt(move.x * move.x + move.y * move.y);
        move /= len;
        move *= MOVE_SPEED * dt;

        sf::Vector2f nextPos = m_position + move;

        // Bounds check
        if (nextPos.x < 0)
            nextPos.x = 0;
        if (nextPos.y < 0)
            nextPos.y = 0;
        if (nextPos.x >= MAP_SIZE)
            nextPos.x = MAP_SIZE - 0.01f;
        if (nextPos.y >= MAP_SIZE)
            nextPos.y = MAP_SIZE - 0.01f;

        int nextGridX = (int)nextPos.x;
        int nextGridY = (int)nextPos.y;
        float obstacleHeight = heights[nextGridX][nextGridY] * BLOCK_HEIGHT;

        if (m_z >= obstacleHeight - 5.0f)
        {
            m_position = nextPos;
        }
    }
}

void Player::update(float dt, int heights[MAP_SIZE][MAP_SIZE])
{
    // Gravity
    int cx = (int)m_position.x;
    int cy = (int)m_position.y;
    float floorLevel = 0.0f;

    if (cx >= 0 && cx < MAP_SIZE && cy >= 0 && cy < MAP_SIZE)
    {
        floorLevel = heights[cx][cy] * BLOCK_HEIGHT;
    }

    m_z += m_velocityZ * dt;
    m_velocityZ -= GRAVITY * dt;

    if (m_z <= floorLevel)
    {
        m_z = floorLevel;
        m_velocityZ = 0;
    }
}

void Player::jump(int heights[MAP_SIZE][MAP_SIZE])
{
    int cx = (int)m_position.x;
    int cy = (int)m_position.y;

    if (cx >= 0 && cx < MAP_SIZE && cy >= 0 && cy < MAP_SIZE)
    {
        float currentFloor = heights[cx][cy] * BLOCK_HEIGHT;
        if (std::abs(m_z - currentFloor) < 5.0f)
        {
            m_velocityZ = JUMP_FORCE;
        }
    }
}
