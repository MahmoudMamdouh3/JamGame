#include "FollowerAnimator.h"

FollowerAnimator::FollowerAnimator(sf::Sprite &spriteRef)
    : CharacterAnimator(spriteRef)
{
}

// TODO: replace with actual asset paths
std::string FollowerAnimator::down_texturePath() const
{
    return "assets/player_down.png";
}
std::string FollowerAnimator::up_texturePath() const
{
    return "assets/player_up.png";
}
std::string FollowerAnimator::left_texturePath() const
{
    return "assets/player_left.png";
}
std::string FollowerAnimator::right_texturePath() const
{
    return "assets/player_right.png";
}