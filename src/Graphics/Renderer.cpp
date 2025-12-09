#include "Renderer.h"
#include "Cube.h"
#include "IsometricUtils.h"
#include <cmath>
#include <algorithm>

Renderer::Renderer(sf::RenderWindow &window)
    : m_window(window), m_showGrid(true)
{
    m_cameraOffset = sf::Vector2f(WINDOW_WIDTH / 2.0f, 100.f);
}

void Renderer::toggleGrid()
{
    m_showGrid = !m_showGrid;
}

sf::Vector2f Renderer::lerp(const sf::Vector2f &start, const sf::Vector2f &end, float t)
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

void Renderer::render(const Map &map, const Player &player, const Follower &follower)
{
    // 1. BACKGROUND COLOR (Unsaturated / Paper White)
    // RGB(245, 245, 245) is a very light, desaturated grey.
    m_window.clear(sf::Color(245, 245, 245));

    for (int sum = 0; sum < MAP_SIZE * 2; sum++)
    {
        for (int x = 0; x < MAP_SIZE; x++)
        {
            int y = sum - x;
            if (y < 0 || y >= MAP_SIZE)
                continue;

            renderTile(map, x, y);

            bool playerHere = ((int)player.getPosition().x == x && (int)player.getPosition().y == y);
            bool followerHere = ((int)follower.getPosition().x == x && (int)follower.getPosition().y == y);

            if (followerHere)
            {
                renderFollower(follower, map, x, y);
            }

            if (playerHere)
            {
                renderPlayer(player, map, x, y);
            }
        }
    }
    m_window.display();
}

void Renderer::renderTile(const Map &map, int x, int y)
{
    if (m_showGrid)
    {
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
        // Transparent so we see the background.
        // If you want a slight floor color, change this to sf::Color(255, 255, 255, 100)
        tile.setFillColor(sf::Color::Cyan);

        // 3. GRID LINE COLOR
        // RGB(160, 160, 160) is a medium-light grey.
        // 100 is the Alpha (transparency) so it isn't too harsh.
        tile.setOutlineColor(sf::Color(160, 160, 160, 150));
        tile.setOutlineThickness(1.0f);

        m_window.draw(tile);
    }
}

void Renderer::renderPlayer(const Player &player, const Map &map, int x, int y)
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

    const sf::Sprite &sprite = player.getSprite();
    sf::Vector2f spritePos = pScreen;
    spritePos.y -= (player.getZ() + currentFloorY);

    sf::Sprite drawSprite = sprite;
    drawSprite.setPosition(sf::Vector2f(std::round(spritePos.x), std::round(spritePos.y)));

    m_window.draw(drawSprite);
}

void Renderer::renderFollower(const Follower &follower, const Map &map, int x, int y)
{
    sf::Vector2f pScreen = IsometricUtils::gridToScreen(follower.getPosition().x, follower.getPosition().y);
    pScreen += m_cameraOffset;
    float currentFloorY = map.getHeight(x, y) * BLOCK_HEIGHT;

    sf::CircleShape shadow = follower.getShadow();
    shadow.setPosition(sf::Vector2f(pScreen.x, pScreen.y - currentFloorY));
    m_window.draw(shadow);

    const sf::Sprite &sprite = follower.getSprite();
    sf::Vector2f spritePos = pScreen;
    spritePos.y -= (follower.getZ() + currentFloorY);
    sf::Sprite drawSprite = sprite;
    drawSprite.setPosition(sf::Vector2f(std::round(spritePos.x), std::round(spritePos.y)));

    m_window.draw(drawSprite);
}

void Renderer::renderFollower(const Follower &follower, const Map &map, int x, int y)
{
    sf::Vector2f pScreen = IsometricUtils::gridToScreen(follower.getPosition().x, follower.getPosition().y);
    pScreen += m_cameraOffset;
    float currentFloorY = map.getHeight(x, y) * BLOCK_HEIGHT;

    sf::CircleShape shadow = follower.getShadow();
    shadow.setPosition(sf::Vector2f(pScreen.x, pScreen.y - currentFloorY));
    m_window.draw(shadow);

    const sf::Sprite &sprite = follower.getSprite();
    sf::Vector2f spritePos = pScreen;
    spritePos.y -= (follower.getZ() + currentFloorY);
    sf::Sprite drawSprite = sprite;
    drawSprite.setPosition(spritePos);
    m_window.draw(drawSprite);
}