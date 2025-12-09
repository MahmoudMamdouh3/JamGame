#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include <optional>
#include "../Audio/AudioManager.h" // Ensure this path is correct

class OptionsMenu
{
private:
    sf::RenderWindow& m_window;
    sf::Font m_font;

    // Options menu state
    int m_selectedOption;
    bool m_isDraggingSlider;

    // UI Elements - Textured
    sf::Texture m_optionsTitleTexture;
    std::optional<sf::Sprite> m_optionsTitleSprite;

    sf::Texture m_soundTexture;
    std::optional<sf::Sprite> m_soundSprite;

    // Volume slider
    sf::Texture m_volumeSliderHandleTexture;
    std::optional<sf::Sprite> m_volumeSliderHandleSprite;
    sf::Texture m_volumeSliderBackgroundTexture;
    std::optional<sf::Sprite> m_volumeSliderBackgroundSprite;

    // SFML 3: Use unique_ptr for Text to avoid initialization issues
    std::unique_ptr<sf::Text> m_volumeValueText;

    float m_sliderTrackLeft = 500.f;
    float m_sliderTrackWidth = 200.f;

    // Fullscreen button
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
    OptionsMenu(sf::RenderWindow& window);

    // Audio passed by reference
    void handleInput(AudioManager& audio);

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

    // Internal helper to update text and audio engine
    void updateVolumeDisplay(AudioManager* audio = nullptr);

    void toggleFullscreen();
    void applyWindowSettings();
};