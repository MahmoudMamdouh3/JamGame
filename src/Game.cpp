#include "Game.h"
#include <iostream>

Game::Game()
    : m_window(sf::VideoMode({ (unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT }), "Isometric Adventure"),
    m_map(),
    m_player(), // Default constructor now works correctly with the fix
    m_renderer(m_window)
{
    m_window.setFramerateLimit(60);

    // Important: Load player assets (texture)
    // This calls the method where we load the real texture into the sprite
    m_player.loadAssets();

    // Build the initial level
    m_map.buildLevel();
}

void Game::run()
{
    while (m_window.isOpen())
    {
        // Calculate Delta Time
        float dt = m_clock.restart().asSeconds();

        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional event = m_window.pollEvent())
    {
        // Handle Window Close
        if (event->is<sf::Event::Closed>())
            m_window.close();

        // Handle Key Presses (One-time events like Jump or Reset)
        if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Escape)
            {
                m_window.close();
            }
            else if (key->code == sf::Keyboard::Key::Space)
            {
                // Pass map to jump logic to check if on ground
                m_player.jump(m_map);
            }
            else if (key->code == sf::Keyboard::Key::R)
            {
                m_map.buildLevel();
            }
        }
    }
}

void Game::update(float dt)
{
    // 1. Handle Continuous Input (Movement)
    m_player.handleInput(dt, m_map);

    // 2. Update Physics/Animation
    m_player.update(dt, m_map);
}

void Game::render()
{
    // Delegate rendering to the Renderer class
    m_renderer.render(m_map, m_player);
}