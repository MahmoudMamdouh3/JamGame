#include "Player.h"
#include "../World/Map.h" 
#include "../Graphics/IsometricUtils.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdint> 

// --- CONFIGURATION ---
const float PLAYER_SCALE = 0.5f;
const float WALK_SPEED = 4.0f;
const float RUN_SPEED = 7.0f;
const float FALL_SPEED = 300.0f;

Player::Player()
    : m_dummyTexture(),
    m_sprite(m_dummyTexture),
    m_animator(m_sprite),
    m_position(0.f, 0.f),
    m_z(0.0f),
    m_stepTimer(0.0f)
{
    if (!m_texture.loadFromFile("assets/player.png")) {
        m_texture.resize({ 32, 32 });
    }
    m_sprite.setTexture(m_texture);

    sf::Vector2u size = m_texture.getSize();
    m_sprite.setOrigin({ size.x / 2.0f, size.y / 2.0f });
    m_sprite.setScale({ PLAYER_SCALE, PLAYER_SCALE });

    m_shadow.setRadius(24.f);
    m_shadow.setScale({ 1.4f, 0.7f });
    m_shadow.setFillColor(sf::Color(0, 0, 0, 100));
    m_shadow.setOrigin({ 24.f, 24.f });
}

void Player::loadAssets() { m_animator.loadAssets(); }

// --- RESTORED HELPER FUNCTION (Leg Collision) ---
bool Player::isValidPosition(float x, float y, const Map& map)
{
    sf::Vector2f screenPos = IsometricUtils::gridToScreen(x, y);

    // THE IMPORTANT OFFSET YOU LIKED
    screenPos.y -= 50.0f;

    if (map.checkPropCollision(screenPos.x, screenPos.y)) {
        return false; // Hit a wall
    }
    return true; // Safe to move
}
// ------------------------------------------------

void Player::update(float dt, Map& map, AudioManager& audio)
{
    // --- A. FALLING ---
    if (m_isFalling) {
        m_fallTimer += dt;
        float shrinkProgress = std::max(0.0f, 1.0f - m_fallTimer * 2.0f);
        float finalScale = shrinkProgress * PLAYER_SCALE;
        m_sprite.setScale({ finalScale, finalScale });
        m_sprite.move({ 0.f, FALL_SPEED * dt });

        sf::Color c = m_sprite.getColor();
        c.a = (std::uint8_t)(255 * shrinkProgress);
        m_sprite.setColor(c);

        if (m_fallTimer > 1.0f) {
            m_isFalling = false;
            m_fallTimer = 0.f;
            sf::Vector2f pixelStart = map.getStartPosition();
            m_position = IsometricUtils::screenToGrid(pixelStart);
            m_sprite.setScale({ PLAYER_SCALE, PLAYER_SCALE });
            m_sprite.setColor(sf::Color::White);
        }
        return;
    }

    // --- B. MOVEMENT ---
    handleInput(dt, map, audio);

    // --- C. TRIGGERS ---
    sf::Vector2f myPos = IsometricUtils::gridToScreen(m_position.x, m_position.y);
    myPos.y += 0.0f;

    for (auto& prop : map.getProps()) {
        if (prop->contains(myPos)) {
            if (prop->isTrap()) {
                m_isFalling = true;
                prop->triggerFall();
                if (!prop->getAudioFile().empty()) audio.playSound(prop->getAudioFile());
                else audio.playSound("fall.wav");
            }
            if (prop->isWin() && !m_levelComplete) {
                m_levelComplete = true;
                if (!prop->getAudioFile().empty()) audio.playSound(prop->getAudioFile());
                else audio.playSound("win.wav");
                m_position = IsometricUtils::screenToGrid(map.getStartPosition());
            }
        }
    }
}

void Player::handleInput(float dt, const Map& map, AudioManager& audio)
{
    sf::Vector2f input(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) input.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) input.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) input.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) input.x += 1.0f;

    float currentSpeed = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) ? RUN_SPEED : WALK_SPEED;

    if (input.x != 0 || input.y != 0)
    {
        float len = std::sqrt(input.x * input.x + input.y * input.y);
        input /= len;
        m_animator.update(dt, input);

        sf::Vector2f velocity = input * currentSpeed * dt;

        // --- FIX: USE isValidPosition (with -50 offset) ---

        // 1. Try X Move
        float nextX = m_position.x + velocity.x;
        if (isValidPosition(nextX, m_position.y, map)) {
            m_position.x = nextX;
        }

        // 2. Try Y Move
        float nextY = m_position.y + velocity.y;
        if (isValidPosition(m_position.x, nextY, map)) {
            m_position.y = nextY;
        }
        // --------------------------------------------------

        // --- AUDIO (One-Time Trigger) ---
        m_stepTimer += dt;
        float stepInterval = (currentSpeed == RUN_SPEED) ? 0.3f : 0.5f;

        if (m_stepTimer >= stepInterval)
        {
            sf::Vector2f myPixelPos = IsometricUtils::gridToScreen(m_position.x, m_position.y);

            for (auto& prop : map.getProps()) {
                if (prop->contains(myPixelPos)) {
                    if (!prop->getAudioFile().empty() && !prop->hasAudioPlayed()) {
                        audio.playSound(prop->getAudioFile());
                        prop->markAudioAsPlayed();
                        break;
                    }
                }
            }
            m_stepTimer = 0.0f;
        }
    }
    else
    {
        m_animator.update(dt, sf::Vector2f(0.f, 0.f));
        m_stepTimer = 0.5f;
    }
}

void Player::render(sf::RenderWindow& window, const sf::Vector2f& cameraOffset)
{
    sf::Vector2f screenPos = IsometricUtils::gridToScreen(m_position.x, m_position.y);
    m_sprite.setPosition(screenPos + cameraOffset + sf::Vector2f(0.f, -16.f));
    window.draw(m_sprite);
}

// GETTERS
sf::Vector2f Player::getPosition() const { return m_position; }
float Player::getZ() const { return m_z; }
const sf::Sprite& Player::getSprite() const { return m_sprite; }
const sf::CircleShape& Player::getShadow() const { return m_shadow; }