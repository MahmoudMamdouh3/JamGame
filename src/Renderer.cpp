#include "Renderer.h"
#include "Cube.h"
#include "IsometricUtils.h"
#include <iostream>

Renderer::Renderer(sf::RenderWindow& window)
    : m_window(window), m_cameraOffset(WINDOW_WIDTH / 2.0f, 100.f)
{
}

void Renderer::render(const Map& map, const Player& player)
{
    // Use the warm white background color
    m_window.clear(sf::Color(245, 240, 230));

    // Isometric Depth Sort Loop
    for (int sum = 0; sum < MAP_SIZE * 2; sum++)
    {
        for (int x = 0; x < MAP_SIZE; x++)
        {
            int y = sum - x;
            if (y < 0 || y >= MAP_SIZE) continue;

            // 1. Draw the terrain/building for this tile
            renderTile(map, x, y);

            // 2. Check if player is on this tile and draw them
            // We cast to int to see if the player occupies this specific grid cell
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
        tile.setFillColor(sf::Color(200, 200, 200)); // Lighter floor
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

            // Block Colors
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

    // 1. Draw Shadow
    sf::CircleShape shadow = player.getShadow();
    shadow.setPosition(sf::Vector2f(pScreen.x, pScreen.y - currentFloorY));
    m_window.draw(shadow);

    // 2. Draw Sprite
    const sf::Sprite& sprite = player.getSprite();

    // Calculate sprite position
    sf::Vector2f spritePos = pScreen;
    spritePos.y -= (player.getZ() + currentFloorY);

    // Create a copy of the sprite to set its position
    // (We must copy because the original sprite inside Player doesn't know its screen coordinates)
    sf::Sprite drawSprite = sprite;
    drawSprite.setPosition(spritePos);
    m_window.draw(drawSprite);
}