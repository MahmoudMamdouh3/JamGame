#include "CharacterAnimator.h"
#include <iostream>
#include <cmath>

CharacterAnimator::CharacterAnimator(sf::Sprite &spriteRef)
    : m_sprite(spriteRef),
      m_animTimer(0.0f),
      m_currentFrame(0),
      m_currentDir(Direction::Forward)
{
}

void CharacterAnimator::loadAssets()
{
    bool success = true;
    if (!m_texForward.loadFromFile(down_texturePath()))
        success = false; // down
    if (!m_texBack.loadFromFile(up_texturePath()))
        success = false; // up
    if (!m_texLeft.loadFromFile(left_texturePath()))
        success = false; // left
    if (!m_texRight.loadFromFile(right_texturePath()))
        success = false; // right

    if (!success)
    {
        std::cerr << "ERROR: Failed to load one or more player sprite sheets!" << std::endl;
    }

    m_sprite.setTexture(m_texForward);

    int frameWidth = m_texForward.getSize().x / FRAMES_PER_SHEET;
    int frameHeight = m_texForward.getSize().y;

    m_sprite.setOrigin(sf::Vector2f(frameWidth / 2.0f, (float)frameHeight));
    m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(frameWidth, frameHeight)));
    m_sprite.setScale(sf::Vector2f(1.0f, 1.0f));
}

void CharacterAnimator::update(float dt, sf::Vector2f inputDir)
{
    bool moving = (inputDir.x != 0 || inputDir.y != 0);
    Direction newDir = m_currentDir;

    if (moving)
    {
        if (std::abs(inputDir.x) > std::abs(inputDir.y))
        {
            if (inputDir.x > 0)
                newDir = Direction::Right;
            else
                newDir = Direction::Left;
        }
        else
        {
            if (inputDir.y > 0)
                newDir = Direction::Forward;
            else
                newDir = Direction::Back;
        }
    }

    // 2. SWITCH TEXTURE
    if (newDir != m_currentDir)
    {
        m_currentDir = newDir;
        m_currentFrame = 0;

        switch (m_currentDir)
        {
        case Direction::Forward:
            m_sprite.setTexture(m_texForward);
            break;
        case Direction::Back:
            m_sprite.setTexture(m_texBack);
            break;
        case Direction::Left:
            m_sprite.setTexture(m_texLeft);
            break;
        case Direction::Right:
            m_sprite.setTexture(m_texRight);
            break;
        default:
            break;
        }
    }

    // 3. ANIMATE
    if (moving)
    {
        m_animTimer += dt;
        if (m_animTimer >= FRAME_TIME)
        {
            m_animTimer = 0.0f;
            m_currentFrame = (m_currentFrame + 1) % FRAMES_PER_SHEET;
        }
    }
    else
    {
        m_currentFrame = 0;
        m_animTimer = 0.0f;
    }

    // 4. APPLY TEXTURE RECT (FIXED FOR SFML 3)
    // getTexture() returns a reference (const sf::Texture&), not a pointer.
    // We assume a texture is loaded because loadAssets() does it.
    const sf::Texture &currentTex = m_sprite.getTexture();

    // Use '.' instead of '->'
    int w = currentTex.getSize().x / FRAMES_PER_SHEET;
    int h = currentTex.getSize().y;

    m_sprite.setOrigin(sf::Vector2f(w / 2.0f, (float)h));

    int left = m_currentFrame * w;
    m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(left, 0), sf::Vector2i(w, h)));
}
