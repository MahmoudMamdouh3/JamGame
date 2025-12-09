// #pragma once
// #include <SFML/Graphics.hpp>
// #include <string>
// #include "Config.h" // Access shared constants like FRAME_WIDTH

// class AnimationComponent
// {
// private:
//     sf::Texture m_texture;
//     sf::Sprite &m_sprite; // Reference to the sprite owned by the Player class

//     // Animation State
//     float m_animTimer;
//     int m_currentFrame;
//     int m_currentRow;
//     bool m_facingLeft;

// public:
//     // Constructor takes a reference to the sprite it will control
//     AnimationComponent(sf::Sprite &spriteRef);

//     // Loads the texture and sets initial sprite settings. Pass a custom path for different characters.
//     void loadAssets(const std::string &texturePath = "assets/adventurer_sheet.png");

//     // Updates the animation frame based on movement direction
// // In AnimationComponent.h
//     void update(float dt, sf::Vector2f inputDir);
// };