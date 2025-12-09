#pragma once
#include "CharacterAnimator.h"

class FollowerAnimator : public CharacterAnimator
{
public:
    explicit FollowerAnimator(sf::Sprite &spriteRef);

protected:
    std::string texturePath() const override;
};
