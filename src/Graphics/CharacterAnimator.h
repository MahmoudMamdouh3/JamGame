#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Config.h"

// Generic character animator shared by player and follower
class CharacterAnimator
{
protected:
    sf::Texture m_texture;
    sf::Sprite &m_sprite;

    float m_animTimer;
    int m_currentFrame;
    int m_currentRow;
    bool m_facingLeft;

    virtual std::string texturePath() const = 0;
    virtual int idleFrames() const { return 4; }
    virtual int runFrames() const { return 6; }

public:
    explicit CharacterAnimator(sf::Sprite &spriteRef);
    virtual ~CharacterAnimator() = default;

    void loadAssets();
    void update(float dt, sf::Vector2f inputDir);
};
