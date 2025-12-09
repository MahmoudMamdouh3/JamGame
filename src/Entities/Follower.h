#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationComponent.h"
#include "Config.h"

class Map;

class Follower
{
private:
    sf::Vector2f m_position;
    float m_z;

    sf::Texture m_dummyTexture;
    sf::Sprite m_sprite;
    sf::CircleShape m_shadow;
    AnimationComponent m_animator;

    sf::Vector2i m_targetTile;
    sf::Vector2f m_targetPos;

public:
    Follower();

    void loadAssets();

    const sf::Vector2f &getPosition() const { return m_position; }
    float getZ() const { return m_z; }
    const sf::Sprite &getSprite() const { return m_sprite; }
    const sf::CircleShape &getShadow() const { return m_shadow; }

    void setTargetTile(const sf::Vector2i &tile);
    void setPosition(const sf::Vector2f &pos);

    void update(float dt, const Map &map);
};
