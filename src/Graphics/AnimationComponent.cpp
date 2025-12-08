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
    // Load the texture (Update path if necessary)
    if (!m_texture.loadFromFile("assets/adventurer_sheet.png")) {
        // Fallback: Create a red square if texture fails
        std::cerr << "Error loading player texture!" << std::endl;
        //sf::Image img;
       // img.create(FRAME_WIDTH, FRAME_HEIGHT, sf::Color::Red);
       // m_texture.loadFromImage(img);
    }

    m_sprite.setTexture(m_texture);

    // Setup Sprite Origin (Feet at bottom center)
    // -4.0f adjustment sinks the feet slightly into the tile for better isometric look
    m_sprite.setOrigin(sf::Vector2f(FRAME_WIDTH / 2.0f, (float)FRAME_HEIGHT - 4.0f));

    m_sprite.setScale(sf::Vector2f(SPRITE_SCALE, SPRITE_SCALE));
    m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
}

void AnimationComponent::update(float dt, sf::Vector2f inputDir) {
    bool moving = (inputDir.x != 0 || inputDir.y != 0);

    // 1. Determine Row (State) and Direction
    if (moving) {
        m_currentRow = 1; // Row 1 is usually "Run" in this spritesheet

        // Direction Logic (Isometric adaptation)
        // X-movement dominates facing direction
        if (inputDir.x < 0) m_facingLeft = true;
        else if (inputDir.x > 0) m_facingLeft = false;
        // If only moving Y
        else if (inputDir.y > 0) m_facingLeft = true;
        else if (inputDir.y < 0) m_facingLeft = false;
    }
    else {
        m_currentRow = 0; // Row 0 is "Idle"
    }

    // 2. Apply Flip
    if (m_facingLeft)
        m_sprite.setScale(sf::Vector2f(-SPRITE_SCALE, SPRITE_SCALE));
    else
        m_sprite.setScale(sf::Vector2f(SPRITE_SCALE, SPRITE_SCALE));

    // 3. Update Frame Timer
    int framesInRow = (m_currentRow == 0) ? 4 : 6; // Idle has 4 frames, Run has 6

    m_animTimer += dt;
    if (m_animTimer >= ANIM_FRAME_TIME) {
        m_animTimer = 0.0f;
        m_currentFrame = (m_currentFrame + 1) % framesInRow;
    }

    // 4. Update Texture Rect
    int left = m_currentFrame * FRAME_WIDTH;
    int top = m_currentRow * FRAME_HEIGHT;
    m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(left, top), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
}