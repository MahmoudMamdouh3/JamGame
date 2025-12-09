#include "FollowerAnimator.h"
#include <cmath>

FollowerAnimator::FollowerAnimator(sf::Sprite &spriteRef)
    : CharacterAnimator(spriteRef)
{
}

// void FollowerAnimator::update(float dt, sf::Vector2f inputDir)
// {
//     bool moving = (inputDir.x != 0 || inputDir.y != 0);
//     Direction newDir = m_currentDir;

//     if (moving)
//     {
//         if (std::abs(inputDir.x) > std::abs(inputDir.y))
//         {
//             if (inputDir.x > 0)
//                 newDir = Direction::Right;
//             else
//                 newDir = Direction::Left;
//         }
//         else
//         {
//             if (inputDir.y > 0)
//                 newDir = Direction::Forward;
//             else
//                 newDir = Direction::Back;
//         }
//     }

//     // Switch texture if direction changed
//     if (newDir != m_currentDir)
//     {
//         m_currentDir = newDir;

//         switch (m_currentDir)
//         {
//         case Direction::Forward:
//             m_sprite.setTexture(m_texForward);
//             break;
//         case Direction::Back:
//             m_sprite.setTexture(m_texBack);
//             break;
//         case Direction::Left:
//             m_sprite.setTexture(m_texLeft);
//             break;
//         case Direction::Right:
//             m_sprite.setTexture(m_texRight);
//             break;
//         default:
//             break;
//         }
//     }

//     // Always use frame 0 for follower (no animation)
//     m_currentFrame = 0;
//     m_animTimer = 0.0f;

//     // Apply texture rect
//     const sf::Texture &currentTex = m_sprite.getTexture();
//     int w = currentTex.getSize().x / FRAMES_PER_SHEET;
//     int h = currentTex.getSize().y;

//     m_sprite.setOrigin(sf::Vector2f(w / 2.0f, (float)h));
//     m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(w, h)));
// }

// TODO: replace with actual asset paths
std::string FollowerAnimator::down_texturePath() const
{
    return "assets/follower_down.png";
}
std::string FollowerAnimator::up_texturePath() const
{
    return "assets/follower_up.png";
}
std::string FollowerAnimator::left_texturePath() const
{
    return "assets/follower_left.png";
}
std::string FollowerAnimator::right_texturePath() const
{
    return "assets/follower_right.png";
}