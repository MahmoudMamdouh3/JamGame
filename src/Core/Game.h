#pragma once
#include <SFML/Graphics.hpp>
#include "../Entities/Player.h"
#include "../Entities/Follower.h"
#include "../World/Map.h"
#include "../Graphics/Renderer.h"
#include "../UI/Menu.h"
#include "../UI/PauseMenu.h"
#include "Config.h" 
#include "../Audio/AudioManager.h"
#include "../Tools/Editor.hpp"          
#include "../Tools/ResourceManager.hpp" 

class Game
{
public:
    Game();
    void run();

private:
    sf::RenderWindow m_window;
    sf::Clock m_clock;

    // Core Game Systems
    // Order matters: Audio should generally be destroyed last, but here it is safe
    // provided entities don't try to play sounds in their destructors.
    AudioManager m_audio;

    Map m_map;
    Player m_player;
    Follower m_follower;
    Renderer m_renderer;

    // UI Systems
    Menu m_menu;
    PauseMenu m_pauseMenu;

    // State
    GameState m_gameState;

    sf::Vector2i m_lastPlayerTile;
    int m_currentLevel = 1; // Start at Level 1
	// Editor System
    ResourceManager m_editorResources; 
    std::unique_ptr<Editor> m_editor;  

    AudioManager m_audioManager; // <--- 2. ADD THIS VARIABLE
    void processEvents();
    void update(float dt);
    void render();
    void renderMenu();
    void renderGame();
};