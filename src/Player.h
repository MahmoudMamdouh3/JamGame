#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationComponent.h"
#include "Config.h"

// Forward declaration so we don't need to include Map.h here
class Map;

class Player
{
private:
    sf::Vector2f m_position; // Grid coordinates (x, y)
    float m_z;               // Vertical position (height)
    float m_velocityZ;

    // Visuals
    sf::Sprite m_sprite;
    sf::CircleShape m_shadow;
    AnimationComponent m_animator; // Handles the sprite animation

public:
    Player();

    void loadAssets();

    // Accessors for the Renderer
    const sf::Vector2f &getPosition() const { return m_position; }
    float getZ() const { return m_z; }
    const sf::Sprite &getSprite() const { return m_sprite; }
    const sf::CircleShape &getShadow() const { return m_shadow; }

    // Logic
    // We pass 'const Map&' so the player can ask the map about heights
    void handleInput(float dt, const Map &map);
    void update(float dt, const Map &map);
    void jump(const Map &map);
};