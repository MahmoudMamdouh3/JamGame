#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <optional>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <cstdlib> // For rand()

// --- CONFIGURATION ---
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const float TILE_WIDTH = 64.f;
const float TILE_HEIGHT = 32.f;
const float BLOCK_HEIGHT = 40.f;
const int MAP_SIZE = 30; // Increased to 30x30 to fill the screen

// Physics
const float GRAVITY = 2000.0f;
const float JUMP_FORCE = 750.0f;
const float MOVE_SPEED = 6.0f; // Slightly faster for larger map

// --- UTILS ---
class IsometricUtils {
public:
    static sf::Vector2f gridToScreen(float x, float y) {
        return sf::Vector2f(
            (x - y) * (TILE_WIDTH / 2.0f),
            (x + y) * (TILE_HEIGHT / 2.0f)
        );
    }
};

// --- VISUALS: CUBE ---
class Cube : public sf::Drawable {
public:
    sf::ConvexShape top, left, right;

    Cube(sf::Vector2f pos, sf::Color color) {
        float hw = TILE_WIDTH / 2.f;
        float hh = TILE_HEIGHT / 2.f;
        float h = BLOCK_HEIGHT;

        // 1. Top Face
        top.setPointCount(4);
        top.setPoint(0, sf::Vector2f(0.f, 0.f));
        top.setPoint(1, sf::Vector2f(hw, hh));
        top.setPoint(2, sf::Vector2f(0.f, hh * 2.f));
        top.setPoint(3, sf::Vector2f(-hw, hh));
        top.setPosition(sf::Vector2f(pos.x, pos.y - h));
        top.setFillColor(color);

        // 2. Left Face
        left.setPointCount(4);
        left.setPoint(0, sf::Vector2f(-hw, hh));
        left.setPoint(1, sf::Vector2f(0.f, hh * 2.f));
        left.setPoint(2, sf::Vector2f(0.f, hh * 2.f + h));
        left.setPoint(3, sf::Vector2f(-hw, hh + h));
        left.setPosition(sf::Vector2f(pos.x, pos.y - h));

        sf::Color dark = color;
        dark.r = static_cast<std::uint8_t>(dark.r * 0.7f);
        dark.g = static_cast<std::uint8_t>(dark.g * 0.7f);
        dark.b = static_cast<std::uint8_t>(dark.b * 0.7f);
        left.setFillColor(dark);

        // 3. Right Face
        right.setPointCount(4);
        right.setPoint(0, sf::Vector2f(0.f, hh * 2.f));
        right.setPoint(1, sf::Vector2f(hw, hh));
        right.setPoint(2, sf::Vector2f(hw, hh + h));
        right.setPoint(3, sf::Vector2f(0.f, hh * 2.f + h));
        right.setPosition(sf::Vector2f(pos.x, pos.y - h));

        sf::Color darkest = color;
        darkest.r = static_cast<std::uint8_t>(darkest.r * 0.5f);
        darkest.g = static_cast<std::uint8_t>(darkest.g * 0.5f);
        darkest.b = static_cast<std::uint8_t>(darkest.b * 0.5f);
        right.setFillColor(darkest);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(left, states);
        target.draw(right, states);
        target.draw(top, states);
    }
};

// --- GAME LOGIC ---
class Game {
private:
    sf::RenderWindow m_window;
    sf::Clock m_clock;

    // Player Data
    sf::Vector2f p_pos;
    float p_z = 0.0f;
    float p_velZ = 0.0f;
    sf::CircleShape p_shape;
    sf::CircleShape p_shadow;

    // Map Data
    int heights[MAP_SIZE][MAP_SIZE];

public:
    Game() : m_window(sf::VideoMode({ (unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT }), "Isometric Fullscreen City") {
        m_window.setFramerateLimit(60);

        // Start player in the middle of the map
        p_pos = sf::Vector2f(MAP_SIZE / 2.0f, MAP_SIZE / 2.0f);

        // Initialize Player Visuals
        p_shape.setRadius(15.f);
        p_shape.setOrigin(sf::Vector2f(15.f, 30.f));
        p_shape.setFillColor(sf::Color::Cyan);

        p_shadow.setRadius(15.f);
        p_shadow.setOrigin(sf::Vector2f(15.f, 15.f));
        p_shadow.setScale(sf::Vector2f(1.f, 0.5f));
        p_shadow.setFillColor(sf::Color(0, 0, 0, 100));

        buildLevel();
    }

    void buildLevel() {
        // 1. Fill ground
        for (int x = 0; x < MAP_SIZE; x++)
            for (int y = 0; y < MAP_SIZE; y++)
                heights[x][y] = 0; // Ground level

        // 2. Random City Generator
        // Create 40 random buildings
        for (int i = 0; i < 40; i++) {
            int bx = rand() % (MAP_SIZE - 4);
            int by = rand() % (MAP_SIZE - 4);
            int w = (rand() % 4) + 1; // Width 1-4
            int h = (rand() % 4) + 1; // Depth 1-4
            int height = (rand() % 3) + 1; // Height 1-3 blocks

            for (int x = bx; x < bx + w; x++) {
                for (int y = by; y < by + h; y++) {
                    if (x < MAP_SIZE && y < MAP_SIZE) {
                        heights[x][y] = height;
                    }
                }
            }
        }

        // Ensure player start position is flat (so you don't spawn inside a wall)
        int center = MAP_SIZE / 2;
        heights[center][center] = 0;
        heights[center + 1][center] = 0;
        heights[center][center + 1] = 0;
        heights[center + 1][center + 1] = 0;
    }

    void run() {
        while (m_window.isOpen()) {
            float dt = m_clock.restart().asSeconds();
            processEvents();
            update(dt);
            render();
        }
    }

    void processEvents() {
        while (const std::optional event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) m_window.close();

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Space) {
                    int cx = (int)p_pos.x;
                    int cy = (int)p_pos.y;

                    if (cx >= 0 && cx < MAP_SIZE && cy >= 0 && cy < MAP_SIZE) {
                        float currentFloor = heights[cx][cy] * BLOCK_HEIGHT;
                        if (std::abs(p_z - currentFloor) < 5.0f) {
                            p_velZ = JUMP_FORCE;
                        }
                    }
                }
                // Reset/Rebuild map on 'R'
                if (key->code == sf::Keyboard::Key::R) {
                    buildLevel();
                }
            }
        }
    }

    void update(float dt) {
        sf::Vector2f move(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) move.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) move.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) move.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) move.x += 1;

        if (move.x != 0 || move.y != 0) {
            float len = std::sqrt(move.x * move.x + move.y * move.y);
            move /= len;
            move *= MOVE_SPEED * dt;

            sf::Vector2f nextPos = p_pos + move;

            // Bounds check
            if (nextPos.x < 0) nextPos.x = 0;
            if (nextPos.y < 0) nextPos.y = 0;
            if (nextPos.x >= MAP_SIZE) nextPos.x = MAP_SIZE - 0.01f;
            if (nextPos.y >= MAP_SIZE) nextPos.y = MAP_SIZE - 0.01f;

            int nextGridX = (int)nextPos.x;
            int nextGridY = (int)nextPos.y;
            float obstacleHeight = heights[nextGridX][nextGridY] * BLOCK_HEIGHT;

            if (p_z >= obstacleHeight - 5.0f) {
                p_pos = nextPos;
            }
        }

        // Gravity
        int cx = (int)p_pos.x;
        int cy = (int)p_pos.y;
        float floorLevel = 0.0f;

        if (cx >= 0 && cx < MAP_SIZE && cy >= 0 && cy < MAP_SIZE) {
            floorLevel = heights[cx][cy] * BLOCK_HEIGHT;
        }

        p_z += p_velZ * dt;
        p_velZ -= GRAVITY * dt;

        if (p_z <= floorLevel) {
            p_z = floorLevel;
            p_velZ = 0;
        }
    }

    void render() {
        m_window.clear(sf::Color(30, 30, 35));

        // --- CAMERA OFFSET ---
        // Center the 30x30 map on a 1920x1080 screen
        // Shift Right: Width/2
        // Shift Down: ~100 pixels padding from top
        sf::Vector2f offset(WINDOW_WIDTH / 2.0f, 100.f);

        for (int sum = 0; sum < MAP_SIZE * 2; sum++) {
            for (int x = 0; x < MAP_SIZE; x++) {
                int y = sum - x;
                if (y < 0 || y >= MAP_SIZE) continue;

                sf::Vector2f isoPos = IsometricUtils::gridToScreen((float)x, (float)y);
                isoPos += offset;

                int tileHeight = heights[x][y];

                // Draw Ground
                if (tileHeight == 0) {
                    sf::ConvexShape tile;
                    tile.setPointCount(4);
                    tile.setPoint(0, sf::Vector2f(0.f, 0.f));
                    tile.setPoint(1, sf::Vector2f(TILE_WIDTH / 2, TILE_HEIGHT / 2));
                    tile.setPoint(2, sf::Vector2f(0.f, TILE_HEIGHT));
                    tile.setPoint(3, sf::Vector2f(-TILE_WIDTH / 2, TILE_HEIGHT / 2));

                    tile.setPosition(isoPos);
                    tile.setFillColor(sf::Color(60, 60, 60));
                    tile.setOutlineColor(sf::Color(80, 80, 80)); // Slightly lighter outline
                    tile.setOutlineThickness(1);
                    m_window.draw(tile);
                }
                else {
                    for (int h = 1; h <= tileHeight; h++) {
                        sf::Vector2f blockPos = isoPos;
                        blockPos.y -= (h - 1) * BLOCK_HEIGHT;

                        // Varied colors for buildings
                        sf::Color c;
                        if (h == 1) c = sf::Color(100, 100, 150);
                        else if (h == 2) c = sf::Color(140, 140, 190);
                        else c = sf::Color(180, 180, 230);

                        Cube block(blockPos, c);
                        m_window.draw(block);
                    }
                }

                // Draw Player
                if ((int)p_pos.x == x && (int)p_pos.y == y) {
                    sf::Vector2f pScreen = IsometricUtils::gridToScreen(p_pos.x, p_pos.y);
                    pScreen += offset;

                    float currentFloorY = heights[x][y] * BLOCK_HEIGHT;

                    p_shadow.setPosition(sf::Vector2f(pScreen.x, pScreen.y - currentFloorY));
                    m_window.draw(p_shadow);

                    p_shape.setPosition(sf::Vector2f(pScreen.x, pScreen.y - p_z));
                    m_window.draw(p_shape);
                }
            }
        }

        m_window.display();
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}