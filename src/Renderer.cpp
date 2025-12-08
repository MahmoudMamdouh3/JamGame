#include "Renderer.h"
#include "Cube.h"
#include "IsometricUtils.h"
#include <iostream>
#include <cmath> // For standard math if needed

Renderer::Renderer(sf::RenderWindow& window)
    : m_window(window)
{
    // Start centered roughly on the map center (15, 15)
    // We can just init this to 0, 0, the update() will fix it in the first frame
    m_cameraOffset = sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
}

// Linear Interpolation helper
sf::Vector2f Renderer::lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t)
{
    return start + (end - start) * t;
}

void Renderer::update(float dt, sf::Vector2f targetGridPos)
{
    // 1. Where is the player on the screen right now (if camera was at 0,0)?
    sf::Vector2f playerScreenPos = IsometricUtils::gridToScreen(targetGridPos.x, targetGridPos.y);

    // 2. We want that position to be at the Center of the Window
    sf::Vector2f screenCenter(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);

    // 3. The target offset is the difference
    sf::Vector2f targetOffset = screenCenter - playerScreenPos;

    // 4. Smoothly move current offset to target offset
    // 5.0f is the "Camera Speed". Higher = Snappier, Lower = Floaty.
    float speed = 5.0f * dt;

    // Clamp speed to 1.0 to prevent overshooting
    if (speed > 1.0f) speed = 1.0f;

    m_cameraOffset = lerp(m_cameraOffset, targetOffset, speed);
}

void Renderer::render(const Map& map, const Player& player)
{
    m_window.clear(sf::Color(245, 240, 230));

    // Optimisation Idea for later: 
    // Only loop through x/y that are actually visible on screen based on m_cameraOffset.
    // For now, we still draw everything.

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
    isoPos += m_cameraOffset; // Apply Camera Shift

    int tileHeight = map.getHeight(x, y);

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
    pScreen += m_cameraOffset; // Apply Camera Shift

    float currentFloorY = map.getHeight(x, y) * BLOCK_HEIGHT;

    sf::CircleShape shadow = player.getShadow();
    shadow.setPosition(sf::Vector2f(pScreen.x, pScreen.y - currentFloorY));
    m_window.draw(shadow);

    const sf::Sprite& sprite = player.getSprite();

    sf::Vector2f spritePos = pScreen;
    spritePos.y -= (player.getZ() + currentFloorY);

    sf::Sprite drawSprite = sprite;
    drawSprite.setPosition(spritePos);
    m_window.draw(drawSprite);
}