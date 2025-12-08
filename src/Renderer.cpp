#include "Renderer.h"
#include "Cube.h"
#include "IsometricUtils.h"
#include <iostream>
#include <cmath> 

Renderer::Renderer(sf::RenderWindow& window)
    : m_window(window)
{
    // Start with the map perfectly centered
    // Map Width = 30 * 64 = 1920 (Matches Window Width)
    // Map Height = 30 * 32 = 960 (Fits in 1080 Window with 120px slack)

    // The visual center of our isometric map is at Grid(15, 15)
    // We want that grid point to be at Screen(960, 540)
    // We will calculate this exact offset dynamically in update()
    m_cameraOffset = sf::Vector2f(WINDOW_WIDTH / 2.0f, 100.f);
}

// Linear Interpolation helper
sf::Vector2f Renderer::lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t)
{
    return start + (end - start) * t;
}

void Renderer::update(float dt, sf::Vector2f playerGridPos)
{
    // --- 1. CALCULATE THE "PERFECT CENTER" ANCHOR ---
    // The center of your 30x30 map is at grid coordinates (14.5, 14.5)
    float centerX = MAP_SIZE / 2.0f - 0.5f;
    float centerY = MAP_SIZE / 2.0f - 0.5f;

    // Convert map center to screen space (unadjusted)
    sf::Vector2f mapCenterIso = IsometricUtils::gridToScreen(centerX, centerY);

    // We want this map center to sit at the Window Center (960, 540)
    sf::Vector2f screenCenter(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);

    // The Base Offset required to lock the map to the center of the screen
    sf::Vector2f baseOffset = screenCenter - mapCenterIso;

    // --- 2. CALCULATE "SUBTLE LEAN" (THE JUICE) ---
    // Get player distance from the map center
    sf::Vector2f playerIso = IsometricUtils::gridToScreen(playerGridPos.x, playerGridPos.y);
    sf::Vector2f diff = mapCenterIso - playerIso; // Vector pointing from Player -> Center

    // "Lean Factor": How much the camera follows the player.
    // 0.00 = Locked static camera
    // 1.00 = Hard follow (player always centered)
    // 0.05 = Subtle "breath" (Camera moves 5% towards player)
    float leanFactor = 0.08f;

    sf::Vector2f targetOffset = baseOffset + (diff * leanFactor);

    // --- 3. SMOOTH MOVEMENT ---
    // Lerp smoothly to this new target so it doesn't jitter
    float smoothSpeed = 3.0f * dt;
    m_cameraOffset = lerp(m_cameraOffset, targetOffset, smoothSpeed);
}

void Renderer::render(const Map& map, const Player& player)
{
    m_window.clear(sf::Color(245, 240, 230));

    for (int sum = 0; sum < MAP_SIZE * 2; sum++)
    {
        for (int x = 0; x < MAP_SIZE; x++)
        {
            int y = sum - x;
            if (y < 0 || y >= MAP_SIZE) continue;

            renderTile(map, x, y);

            if ((int)player.getPosition().x == x && (int)player.getPosition().y == y)
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

    // Draw Ground (Flat Tile)
    if (tileHeight == 0)
    {
        sf::ConvexShape tile;
        tile.setPointCount(4);
        tile.setPoint(0, sf::Vector2f(0.f, 0.f));
        tile.setPoint(1, sf::Vector2f(TILE_WIDTH / 2, TILE_HEIGHT / 2));
        tile.setPoint(2, sf::Vector2f(0.f, TILE_HEIGHT));
        tile.setPoint(3, sf::Vector2f(-TILE_WIDTH / 2, TILE_HEIGHT / 2));

        tile.setPosition(isoPos);
        tile.setFillColor(sf::Color(200, 200, 200));
        tile.setOutlineColor(sf::Color(180, 180, 180));
        tile.setOutlineThickness(1);
        m_window.draw(tile);
    }
    // Draw Building (Stacked Cubes)
    else
    {
        for (int h = 1; h <= tileHeight; h++)
        {
            sf::Vector2f blockPos = isoPos;
            blockPos.y -= (h - 1) * BLOCK_HEIGHT;

            sf::Color c;
            if (h == 1)      c = sf::Color(120, 120, 180);
            else if (h == 2) c = sf::Color(160, 160, 210);
            else             c = sf::Color(200, 200, 240);

            Cube block(blockPos, c);
            m_window.draw(block);
        }
    }
}

void Renderer::renderPlayer(const Player& player, const Map& map, int x, int y)
{
    sf::Vector2f pScreen = IsometricUtils::gridToScreen(player.getPosition().x, player.getPosition().y);
    pScreen += m_cameraOffset;

    float currentFloorY = map.getHeight(x, y) * BLOCK_HEIGHT;

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