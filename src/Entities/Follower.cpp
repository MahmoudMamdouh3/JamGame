#include "Follower.h"
#include "Map.h"
#include <cmath>

Follower::Follower()
    : m_position(0.f, 0.f),
      m_z(0.f),
      m_dummyTexture(),
      m_sprite(m_dummyTexture),
      m_shadow(),
      m_animator(m_sprite),
      m_targetTile(0, 0),
      m_targetPos(0.5f, 0.5f)
{
    m_shadow.setRadius(12.f);
    m_shadow.setScale(sf::Vector2f(1.f, 0.5f));
    m_shadow.setFillColor(sf::Color(0, 0, 0, 80));
    m_shadow.setOrigin(sf::Vector2f(12.f, 12.f));
}

void Follower::loadAssets()
{
    m_animator.loadAssets();
}

void Follower::setTargetTile(const sf::Vector2i &tile)
{
    m_targetTile = tile;
    m_targetPos = sf::Vector2f(static_cast<float>(tile.x) + 0.5f, static_cast<float>(tile.y) + 0.5f);
}

void Follower::setPosition(const sf::Vector2f &pos)
{
    m_position = pos;
}

void Follower::update(float dt, const Map &map)
{
    sf::Vector2f dir = m_targetPos - m_position;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    sf::Vector2f animDir(0.f, 0.f);
    if (dist > 0.01f)
    {
        sf::Vector2f norm = dir / dist;
        float step = FOLLOWER_SPEED * dt;
        if (step >= dist)
        {
            m_position = m_targetPos;
        }
        else
        {
            m_position += norm * step;
        }
        animDir = norm;
    }

    int cx = static_cast<int>(m_position.x);
    int cy = static_cast<int>(m_position.y);
    float floorLevel = map.getHeight(cx, cy) * BLOCK_HEIGHT;
    m_z = floorLevel;

    m_animator.update(dt, animDir);
}
