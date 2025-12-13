#include "Renderer.h"
#include "IsometricUtils.h"
#include <iostream>
#include <algorithm> // Required for sorting

Renderer::Renderer(sf::RenderWindow& window)
    : m_window(window), m_showGrid(true)
{
    // CENTER THE CAMERA
    // This assumes your window is 1920x1080 or similar.
    // If objects are too far right, lower the X value (e.g., to 400.0f).
    m_cameraOffset = sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 4.0f);
}

void Renderer::toggleGrid() { m_showGrid = !m_showGrid; }

void Renderer::update(float dt, sf::Vector2f playerGridPos) {
    // Keeping camera fixed for now to prevent it flying away.
    // You can re-enable smooth follow later if you want.
}

void Renderer::render(const Map& map, const Player& player, const Follower& follower)
{
    // 1. Dark Background (Standard Game Look)
    m_window.clear(sf::Color(20, 20, 30));

    // 2. Collect all props
    const auto& props = map.getProps();
    std::vector<Prop*> sortedProps;
    for (const auto& prop : props) {
        sortedProps.push_back(prop.get());
    }

    // 3. Sort Props by Y Position (Depth Sorting)
    // This ensures objects "lower" on screen are drawn "in front"
    std::sort(sortedProps.begin(), sortedProps.end(), [](Prop* a, Prop* b) {
        return a->getPosition().y < b->getPosition().y;
        });

    // 4. Draw Props
    for (auto* prop : sortedProps) {
        prop->render(m_window, m_cameraOffset);
    }

    // 5. Draw Player & Follower
    // We pass 0,0 because we are drawing directly now, not using the grid loop
    renderPlayer(player, map, 0, 0);
    renderFollower(follower, map, 0, 0);

    m_window.display();
}

// Keep these helper functions for Player drawing
void Renderer::renderTile(const Map& map, int x, int y) { /* unused in this mode */ }

void Renderer::renderPlayer(const Player& player, const Map& map, int x, int y)
{
    // Convert Player Grid Pos -> Screen Pos
    sf::Vector2f pScreen = IsometricUtils::gridToScreen(player.getPosition().x, player.getPosition().y);

    // Apply Camera Offset
    pScreen += m_cameraOffset;

    // Draw Shadow
    sf::CircleShape shadow = player.getShadow();
    shadow.setPosition(sf::Vector2f(pScreen.x, pScreen.y)); // Simplified shadow Y
    m_window.draw(shadow);

    // Draw Player Sprite
    const sf::Sprite& sprite = player.getSprite();
    sf::Vector2f spritePos = pScreen;

    // Apply Z-Height (Jumping)
    spritePos.y -= player.getZ();

    sf::Sprite drawSprite = sprite;
    drawSprite.setPosition(sf::Vector2f(std::round(spritePos.x), std::round(spritePos.y)));

    m_window.draw(drawSprite);
}

void Renderer::renderFollower(const Follower& follower, const Map& map, int x, int y)
{
    sf::Vector2f pScreen = IsometricUtils::gridToScreen(follower.getPosition().x, follower.getPosition().y);
    pScreen += m_cameraOffset;

    sf::CircleShape shadow = follower.getShadow();
    shadow.setPosition(sf::Vector2f(pScreen.x, pScreen.y));
    m_window.draw(shadow);

    const sf::Sprite& sprite = follower.getSprite();
    sf::Vector2f spritePos = pScreen;
    spritePos.y -= follower.getZ();

    sf::Sprite drawSprite = sprite;
    drawSprite.setPosition(sf::Vector2f(std::round(spritePos.x), std::round(spritePos.y)));
    m_window.draw(drawSprite);
}