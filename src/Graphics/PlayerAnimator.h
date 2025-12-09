#pragma once
#include "CharacterAnimator.h"

class PlayerAnimator : public CharacterAnimator
{
public:
    explicit PlayerAnimator(sf::Sprite &spriteRef);

protected:
    std::string texturePath() const override;
};
