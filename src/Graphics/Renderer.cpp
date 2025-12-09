#include "Renderer.h"
#include "Cube.h"
#include "IsometricUtils.h"
#include <cmath> 

Renderer::Renderer(sf::RenderWindow& window)
    : m_window(window)
{
    // Initialize camera to start centered roughly
    m_cameraOffset = sf::Vector2f(WINDOW_WIDTH / 2.0f, 100.f);
}

// Helper function for smooth camera movement
sf::Vector2f Renderer::lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t)
{
    return start + (end - start) * t;
}

void Renderer::update(float dt, sf::Vector2f playerGridPos)
{
    // 1. Calculate where the center of the map is in Isometric space
    float centerX = MAP_SIZE / 2.0f - 0.5f;
    float centerY = MAP_SIZE / 2.0f - 0.5f;
    sf::Vector2f mapCenterIso = IsometricUtils::gridToScreen(centerX, centerY);

    // 2. Determine the screen center
    sf::Vector2f screenCenter(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);

    // 3. Calculate base offset needed to center the map
    sf::Vector2f baseOffset = screenCenter - mapCenterIso;

    // 4. Calculate Player "Lean" 
    // This moves the camera slightly towards the player from the center, 
    // creating a dynamic look rather than a strictly locked camera.
    sf::Vector2f playerIso = IsometricUtils::gridToScreen(playerGridPos.x, playerGridPos.y);
    sf::Vector2f diff = mapCenterIso - playerIso;
    float leanFactor = 0.08f; // How much the camera follows the player (0.0 = static, 1.0 = locked)

    sf::Vector2f targetOffset = baseOffset + (diff * leanFactor);

    // 5. Smoothly interpolate current offset to target
    float smoothSpeed = 3.0f * dt;
    m_cameraOffset = lerp(m_cameraOffset, targetOffset, smoothSpeed);
}

void Renderer::render(const Map& map, const Player& player)
{
    m_window.clear(sf::Color(245, 240, 230)); // Nice off-white background

    // PAINTER'S ALGORITHM LOOP
    // We iterate through diagonals (sum = x + y) to draw back-to-front.
    // This ensures that tiles "behind" are drawn before tiles "in front".
    for (int sum = 0; sum < MAP_SIZE * 2; sum++)
    {
        for (int x = 0; x < MAP_SIZE; x++)
        {
            int y = sum - x;

            // Bounds check
            if (y < 0 || y >= MAP_SIZE) continue;

            // 1. Render the Terrain Block
            renderTile(map, x, y);

            // 2. Render the Player
            // We check if the player occupies this specific tile grid coordinate.
            // Casting to (int) ensures the player is drawn within the depth layer of the tile they are standing on.
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

    // If it's a flat tile (Ground level 0)
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
    // If it's a raised block (Height > 0)
    else {
        // Stack cubes on top of each other
        for (int h = 1; h <= tileHeight; h++) {
            sf::Vector2f blockPos = isoPos;
            // Shift up by block height for every layer
            blockPos.y -= (h - 1) * BLOCK_HEIGHT;

            // Simple gradient coloring for height
            sf::Color c;
            if (h == 1)      c = sf::Color(120, 120, 180); // Base
            else if (h == 2) c = sf::Color(160, 160, 210); // Mid
            else             c = sf::Color(200, 200, 240); // Top

            Cube block(blockPos, c);
            m_window.draw(block);
        }
    }
}

void Renderer::renderPlayer(const Player& player, const Map& map, int x, int y)
{
    // Calculate precise screen position based on float coordinates
    sf::Vector2f pScreen = IsometricUtils::gridToScreen(player.getPosition().x, player.getPosition().y);
    pScreen += m_cameraOffset;

    // Calculate how high the floor is where the player is standing
    float currentFloorY = map.getHeight(x, y) * BLOCK_HEIGHT;

    // 1. Draw Shadow (On the floor surface)
    sf::CircleShape shadow = player.getShadow();
    // Shadow stays at the feet level (minus floor height)
    shadow.setPosition(sf::Vector2f(pScreen.x, pScreen.y - currentFloorY));
    m_window.draw(shadow);

    // 2. Draw Player Sprite
    const sf::Sprite& sprite = player.getSprite();
    sf::Vector2f spritePos = pScreen;

    // Adjust Sprite Y:
    // - Move up by floor height (standing on block)
    // - Move up by Z (Jumping)
    spritePos.y -= (player.getZ() + currentFloorY);

    sf::Sprite drawSprite = sprite;
    drawSprite.setPosition(spritePos);
    m_window.draw(drawSprite);
}