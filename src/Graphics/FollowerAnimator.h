#pragma once
#include "CharacterAnimator.h"

class FollowerAnimator : public CharacterAnimator
{
public:
    explicit FollowerAnimator(sf::Sprite &spriteRef);

protected:
    std::string down_texturePath() const override;
    std::string up_texturePath() const override;
    std::string left_texturePath() const override;
    std::string right_texturePath() const override;
};
