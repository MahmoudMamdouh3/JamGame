#include "CharacterAnimator.h"
#include <iostream>

CharacterAnimator::CharacterAnimator(sf::Sprite &spriteRef)
    : m_sprite(spriteRef),
      m_animTimer(0.0f),
      m_currentFrame(0),
      m_currentRow(0),
      m_facingLeft(false)
{
}

void CharacterAnimator::loadAssets()
{
    if (!m_texture.loadFromFile(texturePath()))
    {
        std::cerr << "Error loading character texture!" << std::endl;
    }

    m_sprite.setTexture(m_texture);
    m_sprite.setOrigin(sf::Vector2f(FRAME_WIDTH / 2.0f, (float)FRAME_HEIGHT));
    m_sprite.setScale(sf::Vector2f(SPRITE_SCALE, SPRITE_SCALE));
    m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
}

void CharacterAnimator::update(float dt, sf::Vector2f inputDir)
{
    bool moving = (inputDir.x != 0 || inputDir.y != 0);

    if (moving)
    {
        if (inputDir.x > 0.1f)
            m_currentRow = 0; // D -> Grid X+ -> Bottom Right
        else if (inputDir.x < -0.1f)
            m_currentRow = 3; // A -> Grid X- -> Top Left
        else if (inputDir.y > 0.1f)
            m_currentRow = 2; // S -> Grid Y+ -> Bottom Left
        else if (inputDir.y < -0.1f)
            m_currentRow = 1; // W -> Grid Y- -> Top Right

        m_animTimer += dt;
        if (m_animTimer >= ANIM_FRAME_TIME)
        {
            m_animTimer = 0.0f;
            m_currentFrame = (m_currentFrame + 1) % FRAMES_PER_ROW;
        }
    }
    else
    {
        m_currentFrame = 0;
        m_animTimer = 0.0f;
    }
    m_sprite.setScale(sf::Vector2f(SPRITE_SCALE, SPRITE_SCALE));

    int left = m_currentFrame * FRAME_WIDTH;
    int top = m_currentRow * FRAME_HEIGHT;

    m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(left, top), sf::Vector2i(FRAME_WIDTH, FRAME_HEIGHT)));
}
