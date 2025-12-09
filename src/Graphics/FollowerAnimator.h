#pragma once
#include "CharacterAnimator.h"

class FollowerAnimator : public CharacterAnimator
{
public:
    FollowerAnimator(sf::Sprite& spriteRef);

    // void update(float dt, sf::Vector2f inputDir) override;

protected:
    // Define the cat/follower specific texture paths here
    virtual std::string down_texturePath() const override;
    virtual std::string up_texturePath() const override;
    virtual std::string left_texturePath() const override;
    virtual std::string right_texturePath() const override;
};