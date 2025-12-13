#include "Follower.h"
#include "../Graphics/IsometricUtils.h"
#include <cmath>
#include <algorithm>
#include <iostream>

// Configuration
//const float FOLLOWER_SPEED = 3.8f;
const float STOP_DISTANCE = 1.2f;

Follower::Follower()
    : m_dummyTexture(),
    m_sprite(m_dummyTexture),
    m_animator(m_sprite),
    m_position(2.0f, 2.0f),
    m_z(0.0f)
{
    if (!m_texture.loadFromFile("assets/follower.png")) {
        m_texture.resize({ 32, 32 });
    }
    m_sprite.setTexture(m_texture);

    sf::Vector2u size = m_texture.getSize();
    m_sprite.setOrigin({ size.x / 2.0f, size.y / 2.0f });

    // Small Ghost Cat
    m_sprite.setScale({ 0.35f, 0.35f });
    m_sprite.setColor(sf::Color(255, 200, 200));

    m_shadow.setRadius(12.f);
    m_shadow.setScale({ 1.4f, 0.7f });
    m_shadow.setFillColor(sf::Color(0, 0, 0, 80));
    m_shadow.setOrigin({ 12.f, 12.f });
}

void Follower::loadAssets() { m_animator.loadAssets(); }
void Follower::setPosition(sf::Vector2f pos) { m_position = pos; }

void Follower::update(float dt, sf::Vector2f targetPos, const Map& map)
{
    sf::Vector2f diff = targetPos - m_position;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (dist > STOP_DISTANCE)
    {
        sf::Vector2f dir = diff / dist;
        sf::Vector2f velocity = dir * FOLLOWER_SPEED * dt;

        // --- FIX: NO COLLISION CHECKS HERE ---
        // Just add velocity directly. It will float through everything.
        m_position += velocity;
        // -------------------------------------

        m_animator.update(dt, dir);
    }
    else
    {
        m_animator.update(dt, sf::Vector2f(0.f, 0.f));
    }
}

void Follower::render(sf::RenderWindow& window, const sf::Vector2f& cameraOffset)
{
    sf::Vector2f screenPos = IsometricUtils::gridToScreen(m_position.x, m_position.y);
    m_sprite.setPosition(screenPos + cameraOffset + sf::Vector2f(0.f, -16.f));
    window.draw(m_sprite);
}

sf::Vector2f Follower::getPosition() const { return m_position; }
float Follower::getZ() const { return m_z; }
const sf::Sprite& Follower::getSprite() const { return m_sprite; }
const sf::CircleShape& Follower::getShadow() const { return m_shadow; }