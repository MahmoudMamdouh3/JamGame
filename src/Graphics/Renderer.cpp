#include "Renderer.h"
#include "Cube.h"
#include "IsometricUtils.h"
#include <cmath> 

Renderer::Renderer(sf::RenderWindow& window)
    : m_window(window), m_showGrid(true) // Default grid ON
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
    // Center map logic
    float centerX = MAP_SIZE / 2.0f - 0.5f;
    float centerY = MAP_SIZE / 2.0f - 0.5f;
    sf::Vector2f mapCenterIso = IsometricUtils::gridToScreen(centerX, centerY);
    sf::Vector2f screenCenter(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
    sf::Vector2f baseOffset = screenCenter - mapCenterIso;

    // CAMERA TWEAK: "Subtle" follow
    // We calculate the difference between Player and Center
    sf::Vector2f playerIso = IsometricUtils::gridToScreen(playerGridPos.x, playerGridPos.y);
    sf::Vector2f diff = mapCenterIso - playerIso;

    // leanFactor: 0.05 is very subtle. 0.2 is loose.
    float leanFactor = 0.1f;

    sf::Vector2f targetOffset = baseOffset + (diff * leanFactor);

    // Smooth lerp
    float smoothSpeed = 4.0f * dt;
    m_cameraOffset = lerp(m_cameraOffset, targetOffset, smoothSpeed);
}

void Renderer::render(const Map& map, const Player& player)
{
    m_window.clear(sf::Color(20, 20, 25)); // Darker BG to make game pop

    // DEPTH SORTING LOOP
    for (int sum = 0; sum < MAP_SIZE * 2; sum++)
    {
        for (int x = 0; x < MAP_SIZE; x++)
        {
            int y = sum - x;
            if (y < 0 || y >= MAP_SIZE) continue;

            renderTile(map, x, y);

            // FIX: Use round() instead of cast to int.
            // If player is at 5.9, they are visually closer to tile 6. 
            // Drawing them at tile 6 ensures they stand "in front" of the tile 5 lines.
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
    sf::Vector2f isoPos = IsometricUtils::gridToScreen((float)x, (float)y);
    isoPos += m_cameraOffset;

    int tileHeight = map.getHeight(x, y);

    // GROUND (Level 0)
    if (tileHeight == 0) {
        if (m_showGrid) {
            sf::ConvexShape tile;
            tile.setPointCount(4);
            tile.setPoint(0, sf::Vector2f(0.f, 0.f));
            tile.setPoint(1, sf::Vector2f(TILE_WIDTH / 2, TILE_HEIGHT / 2));
            tile.setPoint(2, sf::Vector2f(0.f, TILE_HEIGHT));
            tile.setPoint(3, sf::Vector2f(-TILE_WIDTH / 2, TILE_HEIGHT / 2));

            tile.setPosition(isoPos);
            tile.setFillColor(sf::Color::Transparent); // Only draw outline
            tile.setOutlineColor(sf::Color(255, 255, 255, 50)); // Faint white lines
            tile.setOutlineThickness(1);
            m_window.draw(tile);
        }
    }
    // BLOCKS (Height > 0)
    else {
        for (int h = 1; h <= tileHeight; h++) {
            sf::Vector2f blockPos = isoPos;
            blockPos.y -= (h - 1) * BLOCK_HEIGHT;

            sf::Color c;
            if (h == 1)      c = sf::Color(100, 100, 150);
            else if (h == 2) c = sf::Color(140, 140, 190);
            else             c = sf::Color(180, 180, 230);

            Cube block(blockPos, c);
            m_window.draw(block);
        }
    }
}

void Renderer::renderPlayer(const Player& player, const Map& map, int x, int y)
{
    sf::Vector2f pScreen = IsometricUtils::gridToScreen(player.getPosition().x, player.getPosition().y);
    pScreen += m_cameraOffset;

    // We must read the height from the ACTUAL float position to prevent snapping visuals
    int realX = (int)std::round(player.getPosition().x);
    int realY = (int)std::round(player.getPosition().y);

    // Safety clamp
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
    drawSprite.setPosition(spritePos);
    m_window.draw(drawSprite);
}