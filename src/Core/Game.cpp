#include "Game.h"
#include <iostream>

Game::Game()
    : m_window(sf::VideoMode({(unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT}), GAME_TITLE),
      m_map(),
      m_player(),
      m_follower(),
      m_renderer(m_window),
      m_menu(m_window),
      m_pauseMenu(m_window),
      m_gameState(GameState::Menu),
      m_audio(),
      m_lastPlayerTile(0, 0)
{
    m_window.setFramerateLimit(60);

    m_audio.loadSound("jump", "assets/jump.wav");
    m_audio.loadSound("menu_move", "assets/blip.wav");
    m_audio.loadSound("menu_select", "assets/select.wav");

    m_audio.playMusic("assets/city_theme.ogg");

    m_player.loadAssets();
    m_follower.loadAssets();
    m_map.buildLevel();

    sf::Vector2f playerStart = m_player.getPosition();
    m_lastPlayerTile = sf::Vector2i(static_cast<int>(playerStart.x), static_cast<int>(playerStart.y));
    m_follower.setPosition(playerStart);
    m_follower.setTargetTile(m_lastPlayerTile);
}

void Game::run()
{
    while (m_window.isOpen())
    {
        float dt = m_clock.restart().asSeconds();

        if (m_gameState == GameState::Menu)
        {
            m_menu.handleInput(m_audio);

            if (m_menu.isSelectionMade())
            {
                int choice = m_menu.getSelectedOption();
                if (choice == 0)
                    m_gameState = GameState::Playing;
                else if (choice == 1)
                    std::cout << "Options selected" << std::endl;
                else if (choice == 2)
                    m_window.close();

                m_menu.resetSelection();
            }
            m_menu.render();
        }
        else if (m_gameState == GameState::Playing)
        {
            processEvents();
            update(dt);
            render();
        }
        else if (m_gameState == GameState::Paused)
        {
            m_pauseMenu.handleInput(m_audio);

            if (m_pauseMenu.isSelectionMade())
            {
                int choice = m_pauseMenu.getSelectedOption();
                if (choice == 0)
                {
                    m_gameState = GameState::Playing;
                }
                else if (choice == 1)
                {
                    m_map.buildLevel();
                    // Add character reset or other logic as needed
                    m_gameState = GameState::Playing;
                }
                else if (choice == 2)
                {
                    // Options handled internally in PauseMenu, no action needed here
                }
                else if (choice == 3)
                {
                    m_gameState = GameState::Menu;
                    m_menu.resetSelection();
                    // Clear any pending events to prevent input bleed
                    while (m_window.pollEvent())
                    {
                    }
                }
                m_pauseMenu.resetSelection();
            }

            // Render the game one more time to show as paused background
            render();

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
            if (key->code == sf::Keyboard::Key::Escape)
            {
                m_gameState = GameState::Paused;
                m_pauseMenu.resetSelection();
            }
            else if (key->code == sf::Keyboard::Key::Space)
            {
                m_player.jump(m_map, m_audio);
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
    m_audio.update();
    m_player.handleInput(dt, m_map);
    if (m_gameState == GameState::Playing)
    {
        sf::Vector2i currentTile(static_cast<int>(m_player.getPosition().x), static_cast<int>(m_player.getPosition().y));
        if (currentTile != m_lastPlayerTile)
        {
            m_follower.setTargetTile(m_lastPlayerTile);
            m_lastPlayerTile = currentTile;
        }

        m_player.update(dt, m_map);
        m_follower.update(dt, m_map);
        m_renderer.update(dt, m_player.getPosition());

        // // Debug: Print current tile position
        // sf::Vector2f playerPos = m_player.getPosition();
        // std::cout << "Player Tile: (" << (int)playerPos.x << ", " << (int)playerPos.y << ")" << std::endl;
    }
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
    m_renderer.render(m_map, m_player, m_follower);
}