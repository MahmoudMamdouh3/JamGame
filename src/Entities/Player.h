#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "AnimationComponent.h" // FIXED: Correct filename

class Player {
public:
    Player();

    void loadAssets();
    void handleInput(float dt, const Map& map);
    void update(float dt, const Map& map);

    // Getters for Renderer
    sf::Vector2f getPosition() const;
    float getZ() const;
    const sf::Sprite& getSprite() const;
    const sf::CircleShape& getShadow() const;

private:
    // Helper for collision logic
    bool isValidPosition(float x, float y, const Map& map);

    // Graphics
    sf::Texture m_dummyTexture; // Placeholder for sprite init
    sf::Sprite m_sprite;
    AnimationComponent m_animator; // FIXED: Correct class name
    sf::CircleShape m_shadow;

    // State
    sf::Vector2f m_position; // Grid X, Y
    float m_z;               // Vertical Z height

    // Constants
    const float MOVE_SPEED = 4.0f;
    const float BLOCK_HEIGHT = 16.0f;
    const int MAP_SIZE = 10;
};