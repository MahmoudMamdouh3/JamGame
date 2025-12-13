#include "Prop.h"
#include "../Tools/CollisionUtils.hpp"
#include <iostream>
#include <algorithm>

// Constructor with 11 Arguments (Rotation & Scale Vector)
Prop::Prop(float x, float y, const std::string& texturePath,
    sf::Vector2f scale, float rotation,
    const std::vector<sf::Vector2f>& colliderPoints,
    bool isWalkable, bool hasCollider,
    bool isTrap, bool isWin, const std::string& audioFile)
    : m_position(x, y),
    m_sprite(m_texture),
    m_localColliderPoints(colliderPoints),
    m_isWalkable(isWalkable),
    m_hasCollider(hasCollider),
    m_isTrap(isTrap),
    m_isWin(isWin),
    m_audioFile(audioFile),
    m_initialScale(scale)
{
    if (!m_texture.loadFromFile(texturePath)) {
        m_texture.resize({ 32, 32 });
    }
    m_texture.setSmooth(true);
    m_sprite.setTexture(m_texture, true);

    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });

    // Apply Transform
    m_sprite.setScale(scale);
    m_sprite.setRotation(sf::degrees(rotation));
    m_sprite.setPosition(m_position);
}

void Prop::update(float dt) {
    if (m_isFalling) {
        m_fallTimer += dt;
        float progress = std::max(0.0f, 1.0f - m_fallTimer * 2.0f);

        // Scale down both X and Y
        sf::Vector2f currentScale = { m_initialScale.x * progress, m_initialScale.y * progress };
        m_sprite.setScale(currentScale);

        m_position.y += 200.0f * dt;
        m_sprite.setPosition(m_position);

        sf::Color c = m_sprite.getColor();
        c.a = static_cast<uint8_t>(255 * progress);
        m_sprite.setColor(c);
    }
}

void Prop::render(sf::RenderWindow& window, const sf::Vector2f& cameraOffset) {
    if (m_sprite.getColor().a > 0) {
        m_sprite.setPosition(m_position + cameraOffset);
        window.draw(m_sprite);
    }
}

bool Prop::isColliding(sf::Vector2f worldPos) const {
    if (!m_hasCollider || m_isWalkable) return false;
    return contains(worldPos);
}

bool Prop::contains(sf::Vector2f worldPos) const {
    if (!m_hasCollider && !m_isTrap && !m_isWin) return false;
    if (m_localColliderPoints.empty()) return false;

    std::vector<sf::Vector2f> polygonPoints;

    // Transform Points to World Space (Handling Pos, Rot, Scale)
    sf::Transform trans = m_sprite.getTransform();

    // We must subtract the position because isPointInsideConcave usually expects 
    // logic relative to the object's origin if we are doing local checks,
    // BUT here we are transforming local points to WORLD points.

    for (const auto& localPoint : m_localColliderPoints) {
        sf::Vector2f worldPoint = trans.transformPoint(localPoint);
        polygonPoints.push_back(worldPoint);
    }

    return CollisionUtils::isPointInsideConcave(polygonPoints, worldPos);
}