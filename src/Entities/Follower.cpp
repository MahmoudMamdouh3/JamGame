#include "Follower.h"
#include "../World/Map.h"
#include <cmath>
#include <algorithm>
#include <iostream>

Follower::Follower()
    : m_dummyTexture(),
    m_sprite(m_dummyTexture),
    m_animator(m_sprite), // Initializes FollowerAnimator
    m_position(5.0f, 5.0f),
    m_targetTile(5, 5),
    m_z(0.0f)
{
    m_shadow.setRadius(16.f);
    m_shadow.setScale(sf::Vector2f(1.4f, 0.7f));
    m_shadow.setFillColor(sf::Color(0, 0, 0, 80));
    m_shadow.setOrigin(sf::Vector2f(16.f, 16.f));
}

void Follower::loadAssets()
{
    m_animator.loadAssets();
    m_sprite.setColor(sf::Color(255, 200, 200)); // Tint red
}

void Follower::setPosition(sf::Vector2f pos)
{
    m_position = pos;
}

void Follower::setTargetTile(sf::Vector2i tile)
{
    m_targetTile = tile;
}

void Follower::update(float dt, const Map& map)
{
    sf::Vector2f targetPos(static_cast<float>(m_targetTile.x) + 0.5f, static_cast<float>(m_targetTile.y) + 0.5f);
    sf::Vector2f diff = targetPos - m_position;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (dist > 0.1f)
    {
        sf::Vector2f dir = diff / dist;
        m_position += dir * SPEED * dt;
        m_animator.update(dt, dir);
    }
    else
    {
        m_animator.update(dt, sf::Vector2f(0.f, 0.f));
    }

    int cx = std::clamp((int)(m_position.x), 0, MAP_SIZE - 1);
    int cy = std::clamp((int)(m_position.y), 0, MAP_SIZE - 1);

    float targetZ = map.getHeight(cx, cy) * BLOCK_HEIGHT;

    if (m_z < targetZ) m_z += 600.0f * dt;
    else m_z = targetZ;

    if (std::abs(m_z - targetZ) < 2.0f) m_z = targetZ;
}

sf::Vector2f Follower::getPosition() const { return m_position; }
float Follower::getZ() const { return m_z; }
const sf::Sprite& Follower::getSprite() const { return m_sprite; }
const sf::CircleShape& Follower::getShadow() const { return m_shadow; }