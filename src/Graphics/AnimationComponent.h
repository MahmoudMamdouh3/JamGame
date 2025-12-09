#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

enum class Direction {
    Idle,
    Forward, // South-West (Down)
    Back,    // North-East (Up)
    Left,    // North-West
    Right    // South-East
};

class AnimationComponent {
private:
    sf::Sprite& m_sprite;

    // Store textures for each direction
    sf::Texture m_texForward;
    sf::Texture m_texBack;
    sf::Texture m_texLeft;
    sf::Texture m_texRight;

    // Animation State
    float m_animTimer;
    int m_currentFrame;
    Direction m_currentDir;

    // Configuration
    const int FRAMES_PER_SHEET = 8; // Based on your image (I counted 8 sprites)
    const float FRAME_TIME = 0.1f;  // Speed of animation

public:
    AnimationComponent(sf::Sprite& spriteRef);

    void loadAssets();
    void update(float dt, sf::Vector2f inputDir);
};