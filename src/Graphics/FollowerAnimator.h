#pragma once
#include "CharacterAnimator.h" // Ensure this file exists!

class FollowerAnimator : public CharacterAnimator
{
public:
    FollowerAnimator(sf::Sprite& spriteRef);

protected:
    // Define the cat/follower specific texture paths here
    virtual std::string down_texturePath() const override;
    virtual std::string up_texturePath() const override;
    virtual std::string left_texturePath() const override;
    virtual std::string right_texturePath() const override;
};