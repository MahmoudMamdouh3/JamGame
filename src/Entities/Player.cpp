#include "Player.h"
#include "../World/Map.h" // Adjusted path to match your structure
#include <cmath>
#include <algorithm>
#include <iostream>

Player::Player()
    : m_dummyTexture(),
    m_sprite(m_dummyTexture),
    m_animator(m_sprite),
    m_position(5.5f, 5.5f),
    m_z(0.0f),
    m_stepTimer(0.0f) // Initialize timer
{
    m_shadow.setRadius(24.f);
    m_shadow.setScale(sf::Vector2f(1.4f, 0.7f));
    m_shadow.setFillColor(sf::Color(0, 0, 0, 100));
    m_shadow.setOrigin(sf::Vector2f(24.f, 24.f));
}

void Player::loadAssets()
{
    m_animator.loadAssets();
}

// UPDATED: Now accepts AudioManager reference
void Player::handleInput(float dt, const Map& map, AudioManager& audio)
{
    sf::Vector2f input(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        input.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        input.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        input.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        input.x += 1.0f;

    // SPRINT LOGIC
    float currentSpeed = WALK_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)) {
        currentSpeed = RUN_SPEED;
    }

    if (input.x != 0 || input.y != 0)
    {
        float len = std::sqrt(input.x * input.x + input.y * input.y);
        input /= len;

        m_animator.update(dt, input);

        sf::Vector2f velocity = input * currentSpeed * dt;

        // Try X Move
        float nextX = m_position.x + velocity.x;
        if (isValidPosition(nextX, m_position.y, map)) {
            m_position.x = nextX;
        }

        // Try Y Move
        float nextY = m_position.y + velocity.y;
        if (isValidPosition(m_position.x, nextY, map)) {
            m_position.y = nextY;
        }

        m_position.x = std::clamp(m_position.x, 0.0f, (float)MAP_SIZE - 0.1f);
        m_position.y = std::clamp(m_position.y, 0.0f, (float)MAP_SIZE - 0.1f);

        // --- FOOTSTEP AUDIO LOGIC ---
        m_stepTimer += dt;

        // Walk = 0.5s, Run = 0.3s between steps
        float stepInterval = (currentSpeed == RUN_SPEED) ? 0.3f : 0.5f;

        if (m_stepTimer >= stepInterval)
        {
            // Play sound (ensure "step" is loaded in Game.cpp, or change to "jump" to test)
            audio.playSound("step");
            m_stepTimer = 0.0f;
        }
    }
    else
    {
        m_animator.update(dt, sf::Vector2f(0.f, 0.f));
        // Reset timer so next step happens immediately when starting to move
        m_stepTimer = 0.5f;
    }
}

bool Player::isValidPosition(float x, float y, const Map& map)
{
    float margin = 0.2f;

    // 1. Check Map Bounds & Heights (Existing Logic)
    float corners[4][2] = {
        {x - margin, y - margin},
        {x + margin, y - margin},
        {x - margin, y + margin},
        {x + margin, y + margin}
    };

    for (auto& p : corners) {
        int gx = std::clamp((int)p[0], 0, MAP_SIZE - 1);
        int gy = std::clamp((int)p[1], 0, MAP_SIZE - 1);

        float myFloor = map.getHeight((int)m_position.x, (int)m_position.y) * BLOCK_HEIGHT;
        float targetFloor = map.getHeight(gx, gy) * BLOCK_HEIGHT;

        if (targetFloor > myFloor + 10.0f) {
            return false;
        }
    }

    // 2. Check Prop Collision
    if (map.checkPropCollision(x, y)) {
        return false;
    }

    return true;
}

void Player::update(float dt, const Map& map)
{
    int cx = (int)(m_position.x + 0.5f);
    int cy = (int)(m_position.y + 0.5f);

    cx = std::clamp(cx, 0, MAP_SIZE - 1);
    cy = std::clamp(cy, 0, MAP_SIZE - 1);

    float targetZ = map.getHeight(cx, cy) * BLOCK_HEIGHT;

    // Fast snap Z for responsiveness
    if (m_z < targetZ) m_z += 600.0f * dt;
    else m_z = targetZ;

    if (std::abs(m_z - targetZ) < 2.0f) m_z = targetZ;
}

sf::Vector2f Player::getPosition() const { return m_position; }
float Player::getZ() const { return m_z; }
const sf::Sprite& Player::getSprite() const { return m_sprite; }
const sf::CircleShape& Player::getShadow() const { return m_shadow; }