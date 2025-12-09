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
    bool m_isDraggingSlider;

    // UI Elements - Textured
    sf::Texture m_optionsTitleTexture;
    std::optional<sf::Sprite> m_optionsTitleSprite;

    sf::Texture m_soundTexture;
    std::optional<sf::Sprite> m_soundSprite;

    // Volume slider (draggable handle)
    sf::Texture m_volumeSliderHandleTexture;
    std::optional<sf::Sprite> m_volumeSliderHandleSprite;
    sf::Texture m_volumeSliderBackgroundTexture;
    std::optional<sf::Sprite> m_volumeSliderBackgroundSprite;
    std::unique_ptr<sf::Text> m_volumeValueText;
    float m_sliderTrackLeft = 500.f;  // Start position of slider track
    float m_sliderTrackWidth = 200.f; // Width of draggable area

    // Fullscreen button with on/off overlay
    sf::Texture m_fullscreenButtonTexture;
    std::optional<sf::Sprite> m_fullscreenButtonSprite;
    sf::Texture m_onTexture;
    std::optional<sf::Sprite> m_onSprite;
    sf::Texture m_offTexture;
    std::optional<sf::Sprite> m_offSprite;

    // Back button
    sf::Texture m_backButtonTexture;
    std::optional<sf::Sprite> m_backButtonSprite;
    std::optional<sf::Vector2f> m_backButtonBaseScale;

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
    }

    float getVolumeLevel() const { return m_volumeLevel; }
    bool isFullscreen() const { return m_isFullscreen; }

private:
    void setupUI();
    void updateSelection();

    // UPDATED: Takes AudioManager pointer to update music volume in real-time
    void updateVolumeDisplay(AudioManager *audio = nullptr);

    void toggleFullscreen();
    void applyWindowSettings();
};