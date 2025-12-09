#pragma once
#include "CharacterAnimator.h"

class FollowerAnimator : public CharacterAnimator
{
public:
    explicit FollowerAnimator(sf::Sprite &spriteRef);

    // void update(float dt, sf::Vector2f inputDir) override;

protected:
    std::string down_texturePath() const override;
    std::string up_texturePath() const override;
    std::string left_texturePath() const override;
    std::string right_texturePath() const override;
};
