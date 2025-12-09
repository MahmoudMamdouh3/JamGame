#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include <optional>
#include "AudioManager.h" // Required for audio integration

class OptionsMenu
{
private:
    sf::RenderWindow &m_window;
    sf::Font m_font;

    // Options menu state
    int m_selectedOption;
    bool m_isResolutionDropdownOpen;
    int m_selectedResolutionIndex;
    bool m_isDraggingSlider;

    // Available resolutions
    std::vector<sf::Vector2u> m_resolutions;

    // UI Elements
    std::unique_ptr<sf::Text> m_titleText;

    // Volume controls
    std::unique_ptr<sf::Text> m_volumeLabel;
    std::unique_ptr<sf::Text> m_volumeValue;
    sf::RectangleShape m_volumeSlider;
    sf::RectangleShape m_volumeHandle;

    // Fullscreen controls
    std::unique_ptr<sf::Text> m_fullscreenLabel;
    std::unique_ptr<sf::Text> m_fullscreenValue;
    sf::RectangleShape m_fullscreenButton;

    // Resolution controls
    std::unique_ptr<sf::Text> m_resolutionLabel;
    std::unique_ptr<sf::Text> m_resolutionValue;
    sf::RectangleShape m_resolutionButton;
    std::vector<std::unique_ptr<sf::Text>> m_resolutionOptions;
    std::vector<sf::RectangleShape> m_resolutionOptionButtons;

    // Back button
    std::unique_ptr<sf::Text> m_backButtonText;
    sf::RectangleShape m_backButton;

    // Background
    sf::Texture m_backgroundTexture;
    std::optional<sf::Sprite> m_backgroundSprite;

    // Colors
    const sf::Color m_buttonColor = sf::Color::White;
    const sf::Color m_selectedColor = sf::Color::Yellow;
    const sf::Color m_textColor = sf::Color::White;

    // Settings Data
    float m_volumeLevel;
    bool m_isFullscreen;

public:
    OptionsMenu(sf::RenderWindow &window);

    // UPDATED: Now takes AudioManager to play sounds
    void handleInput(AudioManager &audio);

    void render();

    // Returns true if "Back" was selected
    bool isBack() const { return m_selectedOption == 100; }

    void resetSelection()
    {
        m_selectedOption = 0;
        m_isResolutionDropdownOpen = false;
    }

    float getVolumeLevel() const { return m_volumeLevel; }
    bool isFullscreen() const { return m_isFullscreen; }
    sf::Vector2u getCurrentResolution() const { return m_resolutions[m_selectedResolutionIndex]; }

private:
    void setupUI();
    void updateSelection();

    // UPDATED: Takes AudioManager pointer to update music volume in real-time
    void updateVolumeDisplay(AudioManager *audio = nullptr);

    void toggleFullscreen();
    void updateResolutionDisplay();
    void applyWindowSettings();
    std::string resolutionToString(sf::Vector2u res) const;
};