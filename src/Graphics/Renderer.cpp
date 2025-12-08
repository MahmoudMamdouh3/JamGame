#include "Renderer.h"
#include "Cube.h"
#include "IsometricUtils.h"
#include <iostream>
#include <cmath> 

Renderer::Renderer(sf::RenderWindow& window)
    : m_window(window)
{
    m_cameraOffset = sf::Vector2f(WINDOW_WIDTH / 2.0f, 100.f);
}

// FIXED: Added missing Lerp implementation
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
    float leanFactor = 0.08f;

    sf::Vector2f targetOffset = baseOffset + (diff * leanFactor);
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

    if (tileHeight == 0) {
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
    else {
        for (int h = 1; h <= tileHeight; h++) {
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