#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "GameObject.hpp"
#include "ResourceManager.hpp"
#include <vector>
#include <string>
#include <optional>

// --- NEW UI CONSTANTS FOR 1920x1080 ---
const float SIDEBAR_WIDTH = 250.0f;     // Wider asset list
const float INFO_PANEL_WIDTH = 350.0f;  // Wider properties panel
const int MAX_LEVELS = 5;               // How many levels we support

class Editor {
private:
    sf::RenderWindow& m_window;
    ResourceManager& m_resources;

    // Scene Data
    std::vector<GameObject> m_sceneObjects;
    GameObject* m_selectedObject = nullptr;

    // UI Elements
    sf::RectangleShape m_sidebarLeft;
    sf::RectangleShape m_sidebarRight;
    sf::RectangleShape m_topBar; // New bar for Level Selection

    // Assets & Audio
    std::vector<std::pair<sf::Sprite, std::string>> m_paletteIcons;
    std::vector<std::string> m_audioOptions;

    // Level Management
    int m_currentLevelIndex = 1; // Start at Level 1

    // State
    std::string m_currentBrushName = "";
    bool m_isDragging = false;
    sf::Vector2f m_dragOffset;

    // Collider Editing
    bool m_isEditingCollider = false;
    std::vector<sf::Vector2f> m_backupCollider;

    // View Control
    float m_sidebarScroll = 0.0f;

public:
    Editor(sf::RenderWindow& window, ResourceManager& resources);

    void handleEvent(const sf::Event& event);
    void update();
    void render();

    // Global save/load that uses m_currentLevelIndex
    void saveCurrentLevel();
    void loadCurrentLevel();

private:
    void setupUI();
    void setupPalette();
    void setupAudioList();

    void handleMouseClick(sf::Mouse::Button button, sf::Vector2i pixelPos);
    void handleKeyboard(sf::Keyboard::Key key);
    void selectObjectAt(sf::Vector2f pos);

    void drawSidebar();
    void drawProperties();
    void drawLevelSelector(); // New Function

    void cycleAudio();
    void changeLevel(int newLevel);
};

#endif // EDITOR_HPP