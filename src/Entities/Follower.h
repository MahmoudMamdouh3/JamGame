#pragma once
#include <SFML/Graphics.hpp>
#include "../Graphics/FollowerAnimator.h" // Use the new animator
#include "../World/Map.h"

class Follower
{
public:
    Follower();

    void loadAssets();
    void update(float dt, const Map& map);

    void setTargetTile(sf::Vector2i tile);
    void setPosition(sf::Vector2f pos);

    sf::Vector2f getPosition() const;
    float getZ() const;
    const sf::Sprite& getSprite() const;
    const sf::CircleShape& getShadow() const;

private:
    sf::Texture m_dummyTexture;
    sf::Sprite m_sprite;
    sf::CircleShape m_shadow;

    // FIX: Use FollowerAnimator instead of AnimationComponent
    FollowerAnimator m_animator;

    sf::Vector2f m_position;
    sf::Vector2i m_targetTile;
    float m_z;

    const float SPEED = 3.8f;
};