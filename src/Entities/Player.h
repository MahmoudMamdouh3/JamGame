#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "AnimationComponent.h"

class Player {
public:
    Player();

    void loadAssets();
    void handleInput(float dt, const Map& map);
    void update(float dt, const Map& map);

    // Getters
    sf::Vector2f getPosition() const;
    float getZ() const;
    const sf::Sprite& getSprite() const;
    const sf::CircleShape& getShadow() const;

private:
    // Helper for collision logic
    bool isValidPosition(float x, float y, const Map& map);

    // Graphics
    sf::Texture m_dummyTexture;
    sf::Sprite m_sprite;
    AnimationComponent m_animator;
    sf::CircleShape m_shadow;

    // State
    sf::Vector2f m_position;
    float m_z;

    // Constants
    const float WALK_SPEED = 4.0f;
    const float RUN_SPEED = 7.0f; // Shift speed
    const float BLOCK_HEIGHT = 16.0f;
    const int MAP_SIZE = 20;
};