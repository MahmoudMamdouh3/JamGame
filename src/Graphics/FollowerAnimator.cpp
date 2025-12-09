#include "FollowerAnimator.h"

FollowerAnimator::FollowerAnimator(sf::Sprite &spriteRef)
    : CharacterAnimator(spriteRef)
{
}

std::string FollowerAnimator::texturePath() const
{
    return "assets/Cat_Sprite_sheet.png";
}
