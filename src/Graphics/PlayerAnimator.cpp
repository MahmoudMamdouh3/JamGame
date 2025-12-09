#include "PlayerAnimator.h"

PlayerAnimator::PlayerAnimator(sf::Sprite &spriteRef)
    : CharacterAnimator(spriteRef)
{
}

std::string PlayerAnimator::down_texturePath() const
{
    return "assets/player_down.png";
}
std::string PlayerAnimator::up_texturePath() const
{
    return "assets/player_up.png";
}
std::string PlayerAnimator::left_texturePath() const
{
    return "assets/player_left.png";
}
std::string PlayerAnimator::right_texturePath() const
{
    return "assets/player_right.png";
}
