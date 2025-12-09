#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Config.h"

enum class Direction
{
    Idle,
    Forward, // South-West (Down)
    Back,    // North-East (Up)
    Left,    // North-West
    Right    // South-East
};

// Generic character animator shared by player and follower
class CharacterAnimator
{
protected:
    sf::Sprite &m_sprite;

    // Store textures for each direction
    sf::Texture m_texForward;
    sf::Texture m_texBack;
    sf::Texture m_texLeft;
    sf::Texture m_texRight;

    float m_animTimer;
    int m_currentFrame;

    Direction m_currentDir;

    // Configuration
    const int FRAMES_PER_SHEET = 8; // Based on your image (I counted 8 sprites)
    const float FRAME_TIME = 0.1f;  // Speed of animation

    virtual std::string down_texturePath() const = 0;
    virtual std::string up_texturePath() const = 0;
    virtual std::string left_texturePath() const = 0;
    virtual std::string right_texturePath() const = 0;

public:
    explicit CharacterAnimator(sf::Sprite &spriteRef);
    virtual ~CharacterAnimator() = default;

    void loadAssets();
    void update(float dt, sf::Vector2f inputDir);
};
