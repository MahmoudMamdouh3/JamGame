#pragma once
#include <SFML/Graphics.hpp>
#include "../Graphics/FollowerAnimator.h" // <--- 1. CHANGE THIS INCLUDE
#include "../World/Map.h"

class Follower {
public:
    Follower();

    void loadAssets();
    void update(float dt, sf::Vector2f targetPos, const Map& map);
    void render(sf::RenderWindow& window, const sf::Vector2f& cameraOffset);
    void setPosition(sf::Vector2f pos);


    sf::Vector2f getPosition() const;
    float getZ() const;
    const sf::Sprite& getSprite() const;
    const sf::CircleShape& getShadow() const;

private:
    sf::Texture m_dummyTexture;
    sf::Texture m_texture;
    sf::Sprite m_sprite;

    // --- 2. CHANGE THIS VARIABLE TYPE ---
    FollowerAnimator m_animator; // Was "Animator", now "FollowerAnimator"

    sf::CircleShape m_shadow;
    sf::Vector2f m_position;
    float m_z;
};