#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Prop {
public:
    // FIX: Changed 'float scale' to 'sf::Vector2f scale' and added 'float rotation'
    Prop(float x, float y, const std::string& texturePath,
        sf::Vector2f scale, float rotation,
        const std::vector<sf::Vector2f>& colliderPoints,
        bool isWalkable, bool hasCollider,
        bool isTrap, bool isWin, const std::string& audioFile);

    void update(float dt);
    void render(sf::RenderWindow& window, const sf::Vector2f& cameraOffset);

    bool isColliding(sf::Vector2f worldPos) const;
    bool contains(sf::Vector2f worldPos) const;
    void triggerFall() { m_isFalling = true; }

    // Getters
    sf::Vector2f getPosition() const { return m_position; }
    bool isTrap() const { return m_isTrap; }
    bool isWin() const { return m_isWin; }
    const std::string& getAudioFile() const { return m_audioFile; }

    // --- FIX: ONE-TIME AUDIO LOGIC ---
    bool hasAudioPlayed() const { return m_audioPlayed; }
    void markAudioAsPlayed() { m_audioPlayed = true; }

private:
    sf::Vector2f m_position;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    std::vector<sf::Vector2f> m_localColliderPoints;

    bool m_isWalkable;
    bool m_hasCollider;
    bool m_isTrap;
    bool m_isWin;
    std::string m_audioFile;

    bool m_isFalling = false;
    float m_fallTimer = 0.0f;
    sf::Vector2f m_initialScale; // Store as Vector now
    bool m_audioPlayed = false;
};