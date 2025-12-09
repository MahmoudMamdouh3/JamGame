#include "AnimationComponent.h"
#include <iostream>

AnimationComponent::AnimationComponent(sf::Sprite &spriteRef)
    : m_sprite(spriteRef),
      m_animTimer(0.0f),
      m_currentFrame(0),
      m_currentRow(0),
      m_facingLeft(false)
{
}

void AnimationComponent::loadAssets()
{
    // IMPORTANT: Make sure this file is a PNG with transparency!
    if (!m_texture.loadFromFile("assets/SpriteSheet02.png"))
    {
        std::cerr << "Error loading player texture!" << std::endl;
    }

    m_sprite.setTexture(m_texture);

    // Origin at feet (Center X, Bottom Y)
    m_sprite.setOrigin(sf::Vector2f(FRAME_WIDTH / 2.0f, (float)FRAME_HEIGHT));
    m_sprite.setScale(sf::Vector2f(SPRITE_SCALE, SPRITE_SCALE));

    // Start at Row 0, Frame 0 (Bottom Right Idle)
    m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
}

void AnimationComponent::update(float dt, sf::Vector2f inputDir)
{
    bool moving = (inputDir.x != 0 || inputDir.y != 0);

    if (moving)
    {
        // --- 1. DETERMINE ROW BASED ON GRID INPUT ---

        // Priority: Check X movement first, then Y.
        // If moving diagonally (W+D), this logic prioritizes X-axis facing.

        if (inputDir.x > 0.1f)
            m_currentRow = 0; // D -> Grid X+ -> Bottom Right
        else if (inputDir.x < -0.1f)
            m_currentRow = 3; // A -> Grid X- -> Top Left
        else if (inputDir.y > 0.1f)
            m_currentRow = 2; // S -> Grid Y+ -> Bottom Left
        else if (inputDir.y < -0.1f)
            m_currentRow = 1; // W -> Grid Y- -> Top Right

        // --- 2. ANIMATE ---
        m_animTimer += dt;
        if (m_animTimer >= ANIM_FRAME_TIME)
        {
            m_animTimer = 0.0f;
            m_currentFrame = (m_currentFrame + 1) % FRAMES_PER_ROW;
        }
    }
    else
    {
        // --- 3. IDLE ---
        m_currentFrame = 0;
        m_animTimer = 0.0f;
        // Note: We keep m_currentRow as is, so he faces the last direction he walked.
    }

    // --- 4. APPLY TEXTURE RECT ---
    m_sprite.setScale(sf::Vector2f(SPRITE_SCALE, SPRITE_SCALE));

    int left = m_currentFrame * FRAME_WIDTH;
    int top = m_currentRow * FRAME_HEIGHT;

    m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(left, top), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
}