#include "Player.h"
#include "Map.h" 
#include <cmath>
#include <algorithm> 
#include <iostream> 
#include "AudioManager.h" // Needed for jump()

Player::Player()
    : m_dummyTexture(),
    m_sprite(m_dummyTexture),
    m_animator(m_sprite),
    m_position(MAP_SIZE / 2.0f, MAP_SIZE / 2.0f),
    m_z(0.0f),
    m_velocityZ(0.0f)
{
    m_shadow.setRadius(12.f);
    m_shadow.setScale(sf::Vector2f(1.f, 0.5f));
    m_shadow.setFillColor(sf::Color(0, 0, 0, 80));
    m_shadow.setOrigin(sf::Vector2f(12.f, 12.f));
}

void Player::loadAssets() {
    m_animator.loadAssets();
}

void Player::handleInput(float dt, const Map& map)
{
    sf::Vector2f move(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) move.y -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) move.y += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) move.x -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) move.x += 1;

    if (move.x != 0 || move.y != 0) {
        float len = std::sqrt(move.x * move.x + move.y * move.y);
        move /= len;
        sf::Vector2f animDir = move;
        move *= MOVE_SPEED * dt;

        sf::Vector2f nextPos = m_position + move;
        sf::Vector2f checkPos = nextPos;
        if (move.x > 0) checkPos.x += COLLISION_BIAS;
        if (move.x < 0) checkPos.x -= COLLISION_BIAS;
        if (move.y > 0) checkPos.y += COLLISION_BIAS;
        if (move.y < 0) checkPos.y -= COLLISION_BIAS;

        int gridX = std::clamp((int)checkPos.x, 0, MAP_SIZE - 1);
        int gridY = std::clamp((int)checkPos.y, 0, MAP_SIZE - 1);
        float obstacleHeight = map.getHeight(gridX, gridY) * BLOCK_HEIGHT;

        if (m_z >= obstacleHeight - 5.0f) {
            m_position = nextPos;
        }

        m_position.x = std::clamp(m_position.x, 0.0f, (float)MAP_SIZE - 0.1f);
        m_position.y = std::clamp(m_position.y, 0.0f, (float)MAP_SIZE - 0.1f);
        m_animator.update(dt, animDir);
    }
    else {
        m_animator.update(dt, sf::Vector2f(0.f, 0.f));
    }
}

void Player::update(float dt, const Map& map) {
    int cx = (int)m_position.x;
    int cy = (int)m_position.y;
    float floorLevel = map.getHeight(cx, cy) * BLOCK_HEIGHT;
    m_z += m_velocityZ * dt;
    m_velocityZ -= GRAVITY * dt;

    if (m_z <= floorLevel) {
        m_z = floorLevel;
        m_velocityZ = 0;
    }
}

// FIXED: Now accepts AudioManager to play sound
void Player::jump(const Map& map, AudioManager& audio)
{
    int cx = (int)m_position.x;
    int cy = (int)m_position.y;
    float currentFloor = map.getHeight(cx, cy) * BLOCK_HEIGHT;

    if (std::abs(m_z - currentFloor) < 5.0f)
    {
        m_velocityZ = JUMP_FORCE;
        audio.playSound("jump");
    }
}