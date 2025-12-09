#include "AnimationComponent.h"
#include <iostream>

AnimationComponent::AnimationComponent(sf::Sprite& spriteRef)
    : m_sprite(spriteRef),
    m_animTimer(0.0f),
    m_currentFrame(0),
    m_currentRow(0),
    m_facingLeft(false)
{
}

void AnimationComponent::loadAssets() {
    // IMPORTANT: Make sure this file is a PNG with transparency!
    if (!m_texture.loadFromFile("assets/SpriteSheet02.png")) {
        std::cerr << "Error loading player texture!" << std::endl;
    }

    m_sprite.setTexture(m_texture);

    // Origin at feet (Center X, Bottom Y)
    m_sprite.setOrigin(sf::Vector2f(FRAME_WIDTH / 2.0f, (float)FRAME_HEIGHT - 4.0f));
    m_sprite.setScale(sf::Vector2f(SPRITE_SCALE, SPRITE_SCALE));

    // Start at Row 0, Frame 0 (Bottom Right Idle)
    m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
}

void AnimationComponent::update(float dt, sf::Vector2f inputDir) {
    bool moving = (inputDir.x != 0 || inputDir.y != 0);

    if (moving) {
        // --- 1. DETERMINE DIRECTION ROW ---
        // Row 0: Bottom Right ( x > 0, y > 0 )
        // Row 1: Top Right    ( x > 0, y < 0 )
        // Row 2: Bottom Left  ( x < 0, y > 0 )
        // Row 3: Top Left     ( x < 0, y < 0 )

        if (inputDir.x > 0) {
            if (inputDir.y < 0) m_currentRow = 1;      // Up-Right
            else                m_currentRow = 0;      // Down-Right or just Right
        }
        else if (inputDir.x < 0) {
            if (inputDir.y < 0) m_currentRow = 3;      // Up-Left
            else                m_currentRow = 2;      // Down-Left or just Left
        }
        else {
            // Moving purely vertical
            if (inputDir.y < 0) m_currentRow = 3;      // Up (Use Top-Left)
            else                m_currentRow = 0;      // Down (Use Bottom-Right)
        }

        // --- 2. ANIMATE ---
        m_animTimer += dt;
        if (m_animTimer >= ANIM_FRAME_TIME) {
            m_animTimer = 0.0f;
            // Loop through all 8 frames
            m_currentFrame = (m_currentFrame + 1) % FRAMES_PER_ROW;
        }
    }
    else {
        // --- 3. IDLE STATE ---
        // User requested: Row 0 (Bottom Right), Frame 0 to be the "Still" state.

        // Option A: Always revert to Bottom Right when idle
        m_currentRow = 0;
        m_currentFrame = 0;

        // Option B (Better feel): Keep the last direction, but reset to frame 0
        // If you prefer Option A (strict user request), keep the line above.
        // If you want him to face the way he was walking when he stops, comment out 'm_currentRow = 0;'

        m_currentFrame = 0; // Force first frame (Standing still)
        m_animTimer = 0.0f; // Reset timer so he doesn't twitch
    }

    // --- 4. APPLY TEXTURE RECT ---
    // Ensure Scale is positive (No flipping needed anymore since we have 4 rows)
    m_sprite.setScale(sf::Vector2f(SPRITE_SCALE, SPRITE_SCALE));

    int left = m_currentFrame * FRAME_WIDTH;
    int top = m_currentRow * FRAME_HEIGHT;

    m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(left, top), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
}