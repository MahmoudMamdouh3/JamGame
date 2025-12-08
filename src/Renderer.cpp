#include "Renderer.h"
#include "Cube.h"
#include "IsometricUtils.h"
#include <SFML/Graphics.hpp>

Renderer::Renderer(sf::RenderWindow &window)
    : m_window(window), m_cameraOffset(WINDOW_WIDTH / 2.0f, 100.f)
{
}

void Renderer::render(const Map &map, const Player &player)
{
    const sf::View &view = m_window.getView();
    m_cameraOffset.x = view.getSize().x / 2.0f;

    // Center the map vertically within the current view to reduce clipping at lower resolutions.
    const float mapHeight = (MAP_SIZE - 1) * TILE_HEIGHT;
    m_cameraOffset.y = (view.getSize().y - mapHeight) / 2.0f;

    m_window.clear(sf::Color(30, 30, 35));

    for (int sum = 0; sum < MAP_SIZE * 2; sum++)
    {
        for (int x = 0; x < MAP_SIZE; x++)
        {
            int y = sum - x;
            if (y < 0 || y >= MAP_SIZE)
                continue;

            renderTile(map, x, y);

            // Draw Player
            if ((int)player.getPosition().x == x && (int)player.getPosition().y == y)
            {
                renderPlayer(player, map, x, y);
            }
        }
    }
}

void Renderer::renderTile(const Map &map, int x, int y)
{
    sf::Vector2f isoPos = IsometricUtils::gridToScreen((float)x, (float)y);
    isoPos += m_cameraOffset;

    int tileHeight = map.getHeight(x, y);

    // Draw Ground
    if (tileHeight == 0)
    {
        sf::ConvexShape tile;
        tile.setPointCount(4);
        tile.setPoint(0, sf::Vector2f(0.f, 0.f));
        tile.setPoint(1, sf::Vector2f(TILE_WIDTH / 2, TILE_HEIGHT / 2));
        tile.setPoint(2, sf::Vector2f(0.f, TILE_HEIGHT));
        tile.setPoint(3, sf::Vector2f(-TILE_WIDTH / 2, TILE_HEIGHT / 2));

        tile.setPosition(isoPos);
        tile.setFillColor(sf::Color(60, 60, 60));
        tile.setOutlineColor(sf::Color(80, 80, 80));
        tile.setOutlineThickness(1);
        m_window.draw(tile);
    }
    else
    {
        for (int h = 1; h <= tileHeight; h++)
        {
            sf::Vector2f blockPos = isoPos;
            blockPos.y -= (h - 1) * BLOCK_HEIGHT;

            // Varied colors for buildings
            sf::Color c;
            if (h == 1)
                c = sf::Color(100, 100, 150);
            else if (h == 2)
                c = sf::Color(140, 140, 190);
            else
                c = sf::Color(180, 180, 230);

            Cube block(blockPos, c);
            m_window.draw(block);
        }
    }
}

void Renderer::renderPlayer(const Player &player, const Map &map, int x, int y)
{
    sf::Vector2f pScreen = IsometricUtils::gridToScreen(player.getPosition().x, player.getPosition().y);
    pScreen += m_cameraOffset;

    float currentFloorY = map.getHeight(x, y) * BLOCK_HEIGHT;

    // Draw shadow
    sf::CircleShape shadow = player.getShadow();
    shadow.setPosition(sf::Vector2f(pScreen.x, pScreen.y - currentFloorY));
    m_window.draw(shadow);

    // Draw player
    sf::CircleShape shape = player.getShape();
    shape.setPosition(sf::Vector2f(pScreen.x, pScreen.y - player.getZ()));
    m_window.draw(shape);
}
