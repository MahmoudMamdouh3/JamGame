#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Prop {
public:
    Prop(float gridX, float gridY, const std::string& texturePath, float scale = 1.0f);

    void render(sf::RenderWindow& window, const sf::Vector2f& cameraOffset);

    bool isColliding(float playerX, float playerY) const;

    // Getters
    sf::Vector2f getPosition() const { return m_gridPos; }
    const sf::Sprite& getSprite() const { return m_sprite; }

private:
    sf::Vector2f m_gridPos;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
};