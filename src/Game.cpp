#include "Game.h"
#include <iostream>

Game::Game()
    : m_window(sf::VideoMode({(unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT}), GAME_TITLE),
      m_map(),
      m_player(),
      m_renderer(m_window),
      m_menu(m_window),
      m_pauseMenu(m_window),
      m_gameState(GameState::Menu)
{
    m_window.setFramerateLimit(60);

    // Important: Load player assets (texture)
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

        if (m_gameState == GameState::Menu)
        {
            m_menu.handleInput();

            // Check if user made a selection
            if (m_menu.isSelectionMade())
            {
                if (m_menu.getSelectedOption() == 0)
                {
                    // Start button pressed
                    m_gameState = GameState::Playing;
                }
                else if (m_menu.getSelectedOption() == 2)
                {
                    // Exit button pressed
                    m_window.close();
                }
                // Reset menu selection for next time
                m_menu.resetSelection();
            }

            renderMenu();
        }
        else if (m_gameState == GameState::Playing)
        {
            processEvents();
            update(dt);
            renderGame();
            m_window.display();
        }
        else if (m_gameState == GameState::Paused)
        {
            m_pauseMenu.handleInput();

            // Check pause menu selection
            if (m_pauseMenu.isSelectionMade())
            {
                if (m_pauseMenu.getSelectedOption() == 0)
                {
                    // Resume
                    m_gameState = GameState::Playing;
                    m_pauseMenu.resetSelection();
                }
                else if (m_pauseMenu.getSelectedOption() == 1)
                {
                    // Restart
                    m_map.buildLevel();
                    m_gameState = GameState::Playing;
                    m_pauseMenu.resetSelection();
                }
                else if (m_pauseMenu.getSelectedOption() == 2)
                {
                    // Options - open the options menu
                    m_pauseMenu.openOptions();
                    m_pauseMenu.clearSelectionMade();
                }
                else if (m_pauseMenu.getSelectedOption() == 3)
                {
                    // Return to menu
                    m_gameState = GameState::Menu;
                    m_menu.resetSelection();
                    m_pauseMenu.resetSelection();
                }
            }

            // Render game in background with pause menu overlay
            renderGame();
            m_pauseMenu.render();
            m_window.display();
        }
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
        if (const auto *key = event->getIf<sf::Event::KeyPressed>())
        {
            // Pause with Escape
            if (key->code == sf::Keyboard::Key::Escape)
            {
                m_gameState = GameState::Paused;
                m_pauseMenu.resetSelection();
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
    if (m_gameState == GameState::Menu)
    {
        renderMenu();
    }
    else if (m_gameState == GameState::Playing)
    {
        renderGame();
    }
}

void Game::renderMenu()
{
    m_menu.render();
}

void Game::renderGame()
{
    m_renderer.render(m_map, m_player);
}