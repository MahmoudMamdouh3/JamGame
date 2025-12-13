#include "FollowerAnimator.h"

// Constructor: Pass the sprite up to the base class
FollowerAnimator::FollowerAnimator(sf::Sprite& spriteRef)
    : CharacterAnimator(spriteRef)
{
}

// Implement the specific paths for the Follower (e.g., Cat)
std::string FollowerAnimator::down_texturePath() const {
    return "assets/follower_down.png"; // Make sure these files exist!
}

std::string FollowerAnimator::up_texturePath() const {
    return "assets/follower_up.png";
}

std::string FollowerAnimator::left_texturePath() const {
    return "assets/follower_left.png";
}

std::string FollowerAnimator::right_texturePath() const {
    return "assets/follower_right.png";
}