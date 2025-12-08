#include "Game.h"
#include <SFML/Graphics.hpp>
#include <optional>

Game::Game()
    : m_window(sf::VideoMode({(unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT}), GAME_TITLE),
      m_renderer(m_window),
      m_menu(m_window),
      m_pauseMenu(m_window),
      m_gameState(GameState::Menu)
{
    m_window.setFramerateLimit(60);
}

void Game::run()
{
    while (m_window.isOpen())
    {
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
        if (event->is<sf::Event::Closed>())
            m_window.close();

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
                m_player.jump(m_map.getHeights());
            }
            // Reset/Rebuild map on 'R'
            else if (key->code == sf::Keyboard::Key::R)
            {
                m_map.buildLevel();
            }
        }
    }
}

void Game::update(float dt)
{
    m_player.handleInput(dt, m_map.getHeights());
    m_player.update(dt, m_map.getHeights());
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
