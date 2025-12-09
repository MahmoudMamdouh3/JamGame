#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>
#include <string>
#include "OptionsMenu.h"

// Forward declaration prevents circular dependency issues
class AudioManager;

enum class MenuState
{
    Main,
    Options,
    Exit
};

class Menu
{
private:
    sf::RenderWindow& m_window;
    MenuState m_currentState;
    int m_selectedOption;
    bool m_selectionMade;

    // Background
    sf::Texture m_backgroundTexture;
    std::optional<sf::Sprite> m_backgroundSprite;

    // Button textures and sprites
    sf::Texture m_startTexture;
    sf::Texture m_optionsTexture;
    sf::Texture m_exitTexture;
    std::optional<sf::Sprite> m_startSprite;
    std::optional<sf::Sprite> m_optionsSprite;
    std::optional<sf::Sprite> m_exitSprite;
    std::optional<sf::Vector2f> m_startBaseScale;
    std::optional<sf::Vector2f> m_optionsBaseScale;
    std::optional<sf::Vector2f> m_exitBaseScale;

    sf::Vector2u m_lastWindowSize{};

    // Options menu
    std::unique_ptr<OptionsMenu> m_optionsMenu;

    // Colors
    const sf::Color m_buttonColor = sf::Color(220, 220, 220);
    const sf::Color m_selectedColor = sf::Color::White;
    const float m_highlightScale = 1.12f;

public:
    Menu(sf::RenderWindow& window);

    // UPDATED: Now takes AudioManager to play sounds
    void handleInput(AudioManager& audio);

    void render();

    MenuState getCurrentState() const { return m_currentState; }
    int getSelectedOption() const { return m_selectedOption; }
    bool isSelectionMade() const { return m_selectionMade; }

    // Pass-through to options volume
    float getVolume() const { return m_optionsMenu->getVolumeLevel(); }

    void resetSelection()
    {
        m_selectionMade = false;
        m_currentState = MenuState::Main;
    }

private:
    void setupAssets();
    void updateLayout();
    void updateButtonSelection();
    bool isMouseOverButton(const std::optional<sf::Sprite>& button, const sf::Vector2f& mousePos) const;
    void checkMouseClick(const sf::Vector2f& mousePos, AudioManager& audio);
};