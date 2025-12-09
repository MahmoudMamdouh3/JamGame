#include "PlayerAnimator.h"

PlayerAnimator::PlayerAnimator(sf::Sprite &spriteRef)
    : CharacterAnimator(spriteRef)
{
}

std::string PlayerAnimator::texturePath() const
{
    return "assets/SpriteSheet02.png";
}
