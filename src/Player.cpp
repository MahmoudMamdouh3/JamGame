#include "Player.h"
#include "Map.h" // Include Map here to use its methods
#include <cmath>
#include <algorithm> // For std::clamp

Player::Player()
    // Initialize m_sprite first, then pass it to m_animator
    : m_sprite(),
      m_animator(m_sprite),
      m_position(MAP_SIZE / 2.0f, MAP_SIZE / 2.0f),
      m_z(0.0f),
      m_velocityZ(0.0f)
{
    // Initialize Shadow
    m_shadow.setRadius(12.f);
    m_shadow.setScale(sf::Vector2f(1.f, 0.5f));
    m_shadow.setFillColor(sf::Color(0, 0, 0, 80));
    m_shadow.setOrigin(sf::Vector2f(12.f, 12.f));

    // Note: m_animator will handle m_sprite initialization in loadAssets()
}

void Player::loadAssets()
{
    m_animator.loadAssets();
}

void Player::handleInput(float dt, const Map &map)
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
        // Normalize vector
        float len = std::sqrt(move.x * move.x + move.y * move.y);
        move /= len;

        sf::Vector2f animDir = move; // Save direction for animation
        move *= MOVE_SPEED * dt;     // Apply speed

        sf::Vector2f nextPos = m_position + move;

        // --- Collision Logic (with Bias) ---
        sf::Vector2f checkPos = nextPos;
        if (move.x > 0)
            checkPos.x += COLLISION_BIAS;
        if (move.x < 0)
            checkPos.x -= COLLISION_BIAS;
        if (move.y > 0)
            checkPos.y += COLLISION_BIAS;
        if (move.y < 0)
            checkPos.y -= COLLISION_BIAS;

        // Clamp to map bounds
        int gridX = std::clamp((int)checkPos.x, 0, MAP_SIZE - 1);
        int gridY = std::clamp((int)checkPos.y, 0, MAP_SIZE - 1);

        // Get floor height from Map class
        float obstacleHeight = map.getHeight(gridX, gridY) * BLOCK_HEIGHT;

        // Only move if we are high enough (or it's not a wall)
        if (m_z >= obstacleHeight - 5.0f)
        {
            m_position = nextPos;
        }

        // Clamp actual position to map bounds to prevent going off-screen
        m_position.x = std::clamp(m_position.x, 0.0f, (float)MAP_SIZE - 0.1f);
        m_position.y = std::clamp(m_position.y, 0.0f, (float)MAP_SIZE - 0.1f);

        // Update Animation (Running)
        m_animator.update(dt, animDir);
    }
    else
    {
        // Update Animation (Idle)
        m_animator.update(dt, sf::Vector2f(0.f, 0.f));
    }
}

void Player::update(float dt, const Map &map)
{
    // Gravity Logic
    int cx = (int)m_position.x;
    int cy = (int)m_position.y;

    // Get ground height at current position
    float floorLevel = map.getHeight(cx, cy) * BLOCK_HEIGHT;

    // Apply Physics
    m_z += m_velocityZ * dt;
    m_velocityZ -= GRAVITY * dt;

    // Land on ground
    if (m_z <= floorLevel)
    {
        m_z = floorLevel;
        m_velocityZ = 0;
    }
}

void Player::jump(const Map &map)
{
    int cx = (int)m_position.x;
    int cy = (int)m_position.y;

    float currentFloor = map.getHeight(cx, cy) * BLOCK_HEIGHT;

    // Allow jumping if close to the floor
    if (std::abs(m_z - currentFloor) < 5.0f)
    {
        m_velocityZ = JUMP_FORCE;
    }
}