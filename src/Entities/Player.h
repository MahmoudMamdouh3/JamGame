#pragma once
#include <SFML/Graphics.hpp>
#include "../Graphics/PlayerAnimator.h" // Adjusted path if needed, usually just "PlayerAnimator.h" if in same folder or include path
#include "../Core/Config.h"
#include "../World/Map.h"
#include "../Audio/AudioManager.h"

class Player
{
public:
    Player();
    bool m_isFalling = false;
    float m_fallTimer = 0.0f;
    void loadAssets();

    // UPDATED: Now takes AudioManager& to play sounds
    void handleInput(float dt, const Map& map, AudioManager& audio);

    void update(float dt,  Map& map , AudioManager& audio);

    // Getters
    sf::Vector2f getPosition() const;
    float getZ() const;
    const sf::Sprite& getSprite() const;
    const sf::CircleShape& getShadow() const;

    bool isLevelComplete() const { return m_levelComplete; }
    void render(sf::RenderWindow& window, const sf::Vector2f& cameraOffset);
    // NEW: Reset flag
 // In Player.h
    void resetLevel(sf::Vector2f startPos) {
        m_levelComplete = false;
        m_position = startPos;
        m_isFalling = false;
        m_sprite.setColor(sf::Color::White);

        // FIX: Set to 0.5f instead of 1.f
        m_sprite.setScale({ 0.5f, 0.5f });
    }

    sf::Texture m_texture;      // The actual player texture

private:
    bool m_levelComplete = false; // Add this variable
    bool isValidPosition(float x, float y, const Map& map);
        
    // Graphics
    sf::Texture m_dummyTexture;
    sf::Sprite m_sprite;
    PlayerAnimator m_animator;
    sf::CircleShape m_shadow;

    // State
    sf::Vector2f m_position;
    float m_z;

    // Audio State
    float m_stepTimer; // Tracks time between footsteps

    // Constants
    const float WALK_SPEED = 4.0f;
    const float RUN_SPEED = 7.0f; // Shift speed
    const float BLOCK_HEIGHT = 16.0f;
    const int MAP_SIZE = 20;
};