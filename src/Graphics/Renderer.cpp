#include "Renderer.h"
#include "Cube.h"
#include "IsometricUtils.h"
#include <cmath> 
#include <algorithm> 

Renderer::Renderer(sf::RenderWindow& window)
    : m_window(window), m_showGrid(true)
{
    m_cameraOffset = sf::Vector2f(WINDOW_WIDTH / 2.0f, 100.f);
}

void Renderer::toggleGrid() {
    m_showGrid = !m_showGrid;
}

sf::Vector2f Renderer::lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t)
{
    return start + (end - start) * t;
}

void Renderer::update(float dt, sf::Vector2f playerGridPos)
{
    float centerX = MAP_SIZE / 2.0f - 0.5f;
    float centerY = MAP_SIZE / 2.0f - 0.5f;
    sf::Vector2f mapCenterIso = IsometricUtils::gridToScreen(centerX, centerY);
    sf::Vector2f screenCenter(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
    sf::Vector2f baseOffset = screenCenter - mapCenterIso;

    sf::Vector2f playerIso = IsometricUtils::gridToScreen(playerGridPos.x, playerGridPos.y);
    sf::Vector2f diff = mapCenterIso - playerIso;
    float leanFactor = 0.1f;

    sf::Vector2f targetOffset = baseOffset + (diff * leanFactor);
    float smoothSpeed = 4.0f * dt;
    m_cameraOffset = lerp(m_cameraOffset, targetOffset, smoothSpeed);
}

void Renderer::render(const Map& map, const Player& player)
{
    // Background Color
    m_window.clear(sf::Color(245, 245, 245));

    // Get all props once
    const auto& props = map.getProps();

    for (int sum = 0; sum < MAP_SIZE * 2; sum++)
    {
        for (int x = 0; x < MAP_SIZE; x++)
        {
            int y = sum - x;
            if (y < 0 || y >= MAP_SIZE) continue;

            // 1. Render Ground Tile
            renderTile(map, x, y);

            // 2. Render Props on this Tile
            // Loop through all props to find which ones belong to this specific (x, y) tile
            for (const auto& propPtr : props) { // propPtr is a std::unique_ptr<Prop>

                // FIX: Use '->' because it is a pointer
                int propX = (int)std::round(propPtr->getPosition().x);
                int propY = (int)std::round(propPtr->getPosition().y);

                if (propX == x && propY == y) {

                    // Manual Render Logic
                    sf::Vector2f screenPos = IsometricUtils::gridToScreen(propPtr->getPosition().x, propPtr->getPosition().y);
                    screenPos += m_cameraOffset;

                    // FIX: Use '->' to get the sprite
                    sf::Sprite s = propPtr->getSprite();

                    // FIX: Round position to avoid visual glitches
                    s.setPosition(sf::Vector2f(std::round(screenPos.x), std::round(screenPos.y)));

                    m_window.draw(s);
                }
            }

            // 3. Render Player
            int pX = (int)std::round(player.getPosition().x);
            int pY = (int)std::round(player.getPosition().y);

            if (pX == x && pY == y)
            {
                renderPlayer(player, map, x, y);
            }
        }
    }
    m_window.display();
}

void Renderer::renderTile(const Map& map, int x, int y)
{
    if (m_showGrid) {
        sf::Vector2f isoPos = IsometricUtils::gridToScreen((float)x, (float)y);
        isoPos += m_cameraOffset;

        sf::ConvexShape tile;
        tile.setPointCount(4);
        tile.setPoint(0, sf::Vector2f(0.f, 0.f));
        tile.setPoint(1, sf::Vector2f(TILE_WIDTH / 2, TILE_HEIGHT / 2));
        tile.setPoint(2, sf::Vector2f(0.f, TILE_HEIGHT));
        tile.setPoint(3, sf::Vector2f(-TILE_WIDTH / 2, TILE_HEIGHT / 2));

        tile.setPosition(isoPos);

        // 2. TILE FILL COLOR
        // Change this if you want transparent or colored tiles
        tile.setFillColor(sf::Color::Transparent);

        // 3. GRID LINE COLOR
        tile.setOutlineColor(sf::Color(160, 160, 160, 150));
        tile.setOutlineThickness(1.0f);

        m_window.draw(tile);
    }
}

void Renderer::renderPlayer(const Player& player, const Map& map, int x, int y)
{
    sf::Vector2f pScreen = IsometricUtils::gridToScreen(player.getPosition().x, player.getPosition().y);
    pScreen += m_cameraOffset;

    int realX = (int)std::round(player.getPosition().x);
    int realY = (int)std::round(player.getPosition().y);
    realX = std::max(0, std::min(realX, (int)MAP_SIZE - 1));
    realY = std::max(0, std::min(realY, (int)MAP_SIZE - 1));

    float currentFloorY = map.getHeight(realX, realY) * BLOCK_HEIGHT;

    // Shadow
    sf::CircleShape shadow = player.getShadow();
    shadow.setPosition(sf::Vector2f(pScreen.x, pScreen.y - currentFloorY));
    m_window.draw(shadow);

    // Sprite
    const sf::Sprite& sprite = player.getSprite();
    sf::Vector2f spritePos = pScreen;
    spritePos.y -= (player.getZ() + currentFloorY);

    sf::Sprite drawSprite = sprite;
    drawSprite.setPosition(sf::Vector2f(std::round(spritePos.x), std::round(spritePos.y)));

    m_window.draw(drawSprite);
}