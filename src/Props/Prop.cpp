#include "Prop.h"
#include "../Graphics/IsometricUtils.h" // Adjust path if needed (e.g. "IsometricUtils.h")
#include <cmath>
#include <iostream>

Prop::Prop(float gridX, float gridY, const std::string& texturePath, float scale)
    : m_gridPos(gridX, gridY),
    m_sprite(m_texture) // <--- FIX: Initialize Sprite with Texture immediately
{
    // 1. Load the texture
    if (!m_texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load prop: " << texturePath << std::endl;
    }

    // 2. The sprite is already linked to m_texture via the initializer list above.
    // We just need to configure it now.
    m_sprite.setScale(sf::Vector2f(scale, scale));

    // Set Origin to bottom-center
    // We cast to float to ensure vector arithmetic works
    sf::Vector2f texSize((float)m_texture.getSize().x, (float)m_texture.getSize().y);
    m_sprite.setOrigin(sf::Vector2f(texSize.x / 2.0f, texSize.y));
}

void Prop::render(sf::RenderWindow& window, const sf::Vector2f& cameraOffset) {
    sf::Vector2f screenPos = IsometricUtils::gridToScreen(m_gridPos.x, m_gridPos.y);
    screenPos += cameraOffset;

    m_sprite.setPosition(screenPos);
    window.draw(m_sprite);
}

bool Prop::isColliding(float playerX, float playerY) const {
    float dx = playerX - m_gridPos.x;
    float dy = playerY - m_gridPos.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    // Collision radius of 0.5 (half a tile)
    return dist < 0.5f;
}