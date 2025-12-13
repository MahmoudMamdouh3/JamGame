#include "Game.h"
#include <iostream>
#include "../Graphics/IsometricUtils.h" // Ensure this is included at top
Game::Game()
    : m_window(sf::VideoMode({ (unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT }), GAME_TITLE),
    m_audio(),
    m_map(),
    m_player(),
    m_follower(),
    m_renderer(m_window),
    m_menu(m_window),
    m_pauseMenu(m_window),
    m_gameState(GameState::Menu),
    m_lastPlayerTile(0, 0)
{
    m_window.setFramerateLimit(60);

    m_audioManager.loadSound("move_up.wav", "assets/move_up.wav");
    m_audioManager.loadSound("move_down.wav", "assets/move_down.wav");
    m_audioManager.loadSound("move_left.wav", "assets/move_left.wav");
    m_audioManager.loadSound("move_right.wav", "assets/move_right.wav");

    // Game Events
    m_audioManager.loadSound("fall.wav", "assets/fall.wav");
    m_audioManager.loadSound("win.wav", "assets/win.wav");

    // Intro Voice
    m_audioManager.loadSound("intro_voice", "assets/intro.wav");

    // UI / Menu
    m_audioManager.loadSound("jump", "assets/jump.mp3");
    m_audioManager.loadSound("menu_move", "assets/blip.wav");
    m_audioManager.loadSound("menu_select", "assets/select.wav");

    // Ambience (Load first, then Add)
    m_audioManager.loadSound("amb_traffic", "assets/alex_traffic.mp3");
    m_audioManager.loadSound("amb_sea", "assets/alex_sea.mp3");

    m_audioManager.addAmbientSound("amb_traffic");
    m_audioManager.addAmbientSound("amb_sea");

    // Cat Voice (Example)
    m_audioManager.loadSound("vo_L1_intro", "assets/vo_L1_intro.mp3");



    // --- 2. START PLAYING ---
    // Play Intro Voice immediately
    m_audioManager.playSound("intro_voice");

    m_player.loadAssets();
    m_follower.loadAssets();
    m_map.buildLevel(1);


    // --- 3. BUILD LEVEL & PLAYER ---
    m_map.buildLevel(m_currentLevel);

    sf::Vector2f pixelStart = m_map.getStartPosition();
    sf::Vector2f gridStart = IsometricUtils::screenToGrid(pixelStart);
    m_player.resetLevel(gridStart);

    sf::Vector2f playerStart = m_player.getPosition();

    m_lastPlayerTile = sf::Vector2i(static_cast<int>(playerStart.x), static_cast<int>(playerStart.y));
    m_follower.setPosition(playerStart);

    m_audioManager.playSound("intro_voice");

    std::cout << "--- STARTING ASSET LOAD ---" << std::endl;

    for (int i = 1; i <= 21; ++i) {
        // 1. Create the name we want to see in the menu (01, 02... 10)
        std::string displayName = (i < 10 ? "0" : "") + std::to_string(i);

        // 2. Define the path. 
        // Based on your previous setup, it is likely just "assets/number.png"
        std::string path = "assets/" + std::to_string(i) + ".png";

        // 3. Try to load
        // We use a temporary texture to check if the file exists
        sf::Texture temp;
        if (temp.loadFromFile(path)) {
            std::cout << "[SUCCESS] Loaded: " << path << std::endl;

            // It worked! Now actually load it into the manager
            m_editorResources.loadAsset(displayName, path, true);
        }
        else {
            // IT FAILED! Let's try a backup path (maybe it's not in 'assets' folder?)
            std::string backupPath = std::to_string(i) + ".png";
            if (temp.loadFromFile(backupPath)) {
                std::cout << "[FIXED] Found at root: " << backupPath << std::endl;
                m_editorResources.loadAsset(displayName, backupPath, true);
            }
            else {
                // FAILED BOTH PATHS
                std::cerr << "[ERROR] Could not find file! Tried: " << path << " AND " << backupPath << std::endl;
            }
        }
    }
    std::cout << "--- FINISHED ASSET LOAD ---" << std::endl;

    // Create Editor Instance
    m_editor = std::make_unique<Editor>(m_window, m_editorResources);
    m_editor->loadCurrentLevel();
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
                if (choice == 0) {
                    m_gameState = GameState::Playing;
                    // m_audio.playDialogue("vo_L1_intro"); 
                }
                else if (choice == 2) {
                    m_gameState = GameState::Editor;
                    // Reset View just in case
                    m_window.setView(m_window.getDefaultView());
                }

                else if (choice == 3) m_window.close();

                m_menu.resetSelection();
            }
            m_menu.render();
        }
        else if (m_gameState == GameState::Editor) {
            while (const std::optional event = m_window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) m_window.close();

                if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    // 1. EXIT EDITOR (Escape)
                    if (key->code == sf::Keyboard::Key::Escape && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
                        m_gameState = GameState::Menu;
                        m_menu.resetSelection();
                        m_window.setView(m_window.getDefaultView()); // Reset camera
                    }

                    // 2. SAVE MAP (F5) <--- ADD THIS BLOCK
                    if (key->code == sf::Keyboard::Key::F5) {
                        m_editor->saveCurrentLevel();
                        std::cout << "[GAME] Map saved to level1.json" << std::endl;
                    }
                }

                m_editor->handleEvent(*event);
            }

            m_editor->update();

            m_window.clear(sf::Color(50, 50, 50));
            m_editor->render();
            m_window.display();
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
                if (choice == 0) m_gameState = GameState::Playing;
                else if (choice == 1) {
                    m_map.buildLevel(m_currentLevel);
                    m_gameState = GameState::Playing;
                }
                else if (choice == 3) {
                    m_gameState = GameState::Menu;
                    m_menu.resetSelection();
                    while (m_window.pollEvent()) {}
                }
                m_pauseMenu.resetSelection();
            }
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

        if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Escape)
            {
                m_gameState = GameState::Paused;
                m_pauseMenu.resetSelection();
            }
            else if (key->code == sf::Keyboard::Key::T)
            {
                m_renderer.toggleGrid();
            }
            else if (key->code == sf::Keyboard::Key::R)
            {
                m_map.buildLevel(m_currentLevel);
            }
        }
    }
}

void Game::update(float dt)
{
    // 1. Update Map (Falling animations)
    m_map.update(dt);

    // 2. CHECK LEVEL COMPLETION
    if (m_player.isLevelComplete()) {
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "[GAME] WIN DETECTED! Current Level: " << m_currentLevel << std::endl;

        // Increment Level
        m_currentLevel++;

        std::cout << "[GAME] Loading Level " << m_currentLevel << "..." << std::endl;

        // Build the Next Level
        m_map.buildLevel(m_currentLevel);

        // Check if map loaded anything (if props is empty, level 2 probably doesn't exist)
        if (m_map.getProps().empty()) {
            std::cout << "[ERROR] Level " << m_currentLevel << " failed to load or is empty!" << std::endl;
            std::cout << "[GAME] Restarting Level 1..." << std::endl;
            m_currentLevel = 1;
            m_map.buildLevel(m_currentLevel);
        }

        sf::Vector2f pixelStart = m_map.getStartPosition();
        sf::Vector2f gridStart = IsometricUtils::screenToGrid(pixelStart);

        m_player.resetLevel(gridStart); // Now passing GRID coords!
        std::cout << "-----------------------------------" << std::endl;
    }

    // 3. Update Player & Audio
    m_player.update(dt, m_map, m_audioManager);
    m_follower.update(dt, m_player.getPosition(), m_map);
}

void Game::render()
{
    if (m_gameState == GameState::Menu) renderMenu();
    else if (m_gameState == GameState::Playing) renderGame();
}

void Game::renderGame()
{
    m_window.clear(sf::Color(20, 20, 20));
    sf::Vector2f cameraOffset = { 0.f, 0.f };
    m_map.render(m_window, cameraOffset);
    m_player.render(m_window, cameraOffset);
    m_follower.render(m_window, cameraOffset);
    m_window.display();
}

void Game::renderMenu() { m_menu.render(); } 