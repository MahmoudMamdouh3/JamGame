#include "OptionsMenu.h"
#include "../Core/Config.h"
#include <algorithm>
#include <iostream>
#include <sstream>

OptionsMenu::OptionsMenu(sf::RenderWindow &window)
    : m_window(window), m_selectedOption(0), m_isDraggingSlider(false),
      m_volumeLevel(50.f), m_isFullscreen(false)
{
    // Try to load font from Config path, fallback to system path
    if (!m_font.openFromFile(FONT_PATH))
    {
        if (!m_font.openFromFile("C:\\Windows\\Fonts\\arial.ttf"))
            std::cerr << "Failed to load font" << std::endl;
    }

    // Background
    if (m_backgroundTexture.loadFromFile("assets/UI/UI Background.png"))
    {
        m_backgroundTexture.setSmooth(true);
        m_backgroundSprite.emplace(m_backgroundTexture);
    }

    setupUI();
}

void OptionsMenu::setupUI()
{
    auto loadTexture = [](sf::Texture &texture, const std::string &path)
    {
        if (!texture.loadFromFile(path))
            std::cerr << "Failed to load texture: " << path << std::endl;
        else
            texture.setSmooth(true);
    };

    // Load all textures
    loadTexture(m_optionsTitleTexture, "assets/UI/Options.png");
    loadTexture(m_soundTexture, "assets/UI/Sound.png");
    loadTexture(m_volumeSliderHandleTexture, "assets/UI/Volume Slider.png");
    loadTexture(m_volumeSliderBackgroundTexture, "assets/UI/Button.png");
    loadTexture(m_fullscreenButtonTexture, "assets/UI/Button.png");
    loadTexture(m_onTexture, "assets/UI/on.png");
    loadTexture(m_offTexture, "assets/UI/off.png");
    loadTexture(m_backButtonTexture, "assets/UI/Exit Button.png");

    // Create sprites
    if (m_optionsTitleTexture.getSize().x > 0)
        m_optionsTitleSprite.emplace(m_optionsTitleTexture);
    if (m_soundTexture.getSize().x > 0)
        m_soundSprite.emplace(m_soundTexture);
    if (m_volumeSliderHandleTexture.getSize().x > 0)
        m_volumeSliderHandleSprite.emplace(m_volumeSliderHandleTexture);
    if (m_volumeSliderBackgroundTexture.getSize().x > 0)
        m_volumeSliderBackgroundSprite.emplace(m_volumeSliderBackgroundTexture);
    if (m_fullscreenButtonTexture.getSize().x > 0)
        m_fullscreenButtonSprite.emplace(m_fullscreenButtonTexture);
    if (m_onTexture.getSize().x > 0)
        m_onSprite.emplace(m_onTexture);
    if (m_offTexture.getSize().x > 0)
        m_offSprite.emplace(m_offTexture);
    if (m_backButtonTexture.getSize().x > 0)
    {
        m_backButtonSprite.emplace(m_backButtonTexture);
        m_backButtonBaseScale = m_backButtonSprite->getScale();
    }

    // Create volume value text
    m_volumeValueText = std::make_unique<sf::Text>(m_font, "50", 30);
    m_volumeValueText->setFillColor(sf::Color::White);

    // Position sprites with increased vertical spacing and aligned x position
    float alignedX = 200.f;
    if (m_optionsTitleSprite)
        m_optionsTitleSprite->setPosition({alignedX, 80.f});
    if (m_soundSprite)
        m_soundSprite->setPosition({alignedX, 200.f});
    // Volume slider line
    if (m_volumeSliderBackgroundSprite)
        m_volumeSliderBackgroundSprite->setPosition({m_sliderTrackLeft, 280.f});
    if (m_volumeSliderHandleSprite)
    {
        // Position handle based on current volume level
        float handleX = m_sliderTrackLeft + (m_volumeLevel / 100.f) * m_sliderTrackWidth;
        m_volumeSliderHandleSprite->setPosition({handleX, 290.f});
    }
    if (m_volumeValueText)
        m_volumeValueText->setPosition({m_sliderTrackLeft + 220.f, 290.f});
    if (m_fullscreenButtonSprite)
        m_fullscreenButtonSprite->setPosition({alignedX, 380.f});
    if (m_onSprite)
        m_onSprite->setPosition({alignedX, 380.f});
    if (m_offSprite)
        m_offSprite->setPosition({alignedX, 380.f});
    if (m_backButtonSprite)
        m_backButtonSprite->setPosition({alignedX, 520.f});
}

void OptionsMenu::updateSelection()
{
    // Update colors for selection state
    if (m_backButtonSprite && m_backButtonBaseScale)
    {
        if (m_selectedOption == 100)
        {
            m_backButtonSprite->setColor(sf::Color::White);
            m_backButtonSprite->setScale(sf::Vector2f{m_backButtonBaseScale->x * 1.1f, m_backButtonBaseScale->y * 1.1f});
        }
        else
        {
            m_backButtonSprite->setColor(sf::Color(220, 220, 220));
            m_backButtonSprite->setScale(*m_backButtonBaseScale);
        }
    }

    // Handle fullscreen button highlight
    if (m_selectedOption == 1)
    {
        if (m_fullscreenButtonSprite)
            m_fullscreenButtonSprite->setColor(sf::Color::White);
    }
    else
    {
        if (m_fullscreenButtonSprite)
            m_fullscreenButtonSprite->setColor(sf::Color(220, 220, 220));
    }

    // Handle volume slider highlight
    if (m_selectedOption == 0 && m_volumeSliderHandleSprite)
        m_volumeSliderHandleSprite->setColor(sf::Color::White);
    else if (m_volumeSliderHandleSprite)
        m_volumeSliderHandleSprite->setColor(sf::Color(220, 220, 220));
}

// UPDATED: Now processes Audio
void OptionsMenu::handleInput(AudioManager &audio)
{
    while (const std::optional event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            m_window.close();

        if (const auto *key = event->getIf<sf::Event::KeyPressed>())
        {
            // Main Options Navigation
            if (key->code == sf::Keyboard::Key::Up)
            {
                // Cycle: 0(Vol) -> 1(Full) -> 100(Back)
                if (m_selectedOption == 0)
                    m_selectedOption = 100;
                else if (m_selectedOption == 100)
                    m_selectedOption = 1;
                else if (m_selectedOption == 1)
                    m_selectedOption = 0;

                audio.playSound("menu_move");
            }
            else if (key->code == sf::Keyboard::Key::Down)
            {
                if (m_selectedOption == 0)
                    m_selectedOption = 1;
                else if (m_selectedOption == 1)
                    m_selectedOption = 100;
                else if (m_selectedOption == 100)
                    m_selectedOption = 0;

                audio.playSound("menu_move");
            }
            // Volume Adjustment
            else if (key->code == sf::Keyboard::Key::Left && m_selectedOption == 0)
            {
                m_volumeLevel = std::max(0.f, m_volumeLevel - 5.f);
                updateVolumeDisplay(&audio);
                audio.playSound("menu_move");
            }
            else if (key->code == sf::Keyboard::Key::Right && m_selectedOption == 0)
            {
                m_volumeLevel = std::min(100.f, m_volumeLevel + 5.f);
                updateVolumeDisplay(&audio);
                audio.playSound("menu_move");
            }
            else if (key->code == sf::Keyboard::Key::Enter || key->code == sf::Keyboard::Key::Space)
            {
                if (m_selectedOption == 1)
                {
                    toggleFullscreen();
                    audio.playSound("menu_select");
                }
                else if (m_selectedOption == 100)
                {
                    audio.playSound("menu_select");
                    // Caller handles state change via isBack()
                }
            }
            else if (key->code == sf::Keyboard::Key::Escape)
            {
                m_selectedOption = 100;
            }
        }

        // Mouse Handling
        if (const auto *mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButton->button == sf::Mouse::Button::Left)
            {
                const sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(mouseButton->position.x, mouseButton->position.y));

                if (m_volumeSliderHandleSprite && m_volumeSliderHandleSprite->getGlobalBounds().contains(mousePos))
                {
                    m_isDraggingSlider = true;
                    m_selectedOption = 0;
                    // Update position immediately on click
                    float clampedX = std::max(m_sliderTrackLeft, std::min(mousePos.x, m_sliderTrackLeft + m_sliderTrackWidth));
                    m_volumeLevel = ((clampedX - m_sliderTrackLeft) / m_sliderTrackWidth) * 100.f;
                    m_volumeSliderHandleSprite->setPosition({clampedX, 290.f});
                    updateVolumeDisplay(&audio);
                    audio.playSound("menu_select");
                }
                else if (m_fullscreenButtonSprite && m_fullscreenButtonSprite->getGlobalBounds().contains(mousePos))
                {
                    m_selectedOption = 1;
                    toggleFullscreen();
                    audio.playSound("menu_select");
                }
                else if (m_backButtonSprite && m_backButtonSprite->getGlobalBounds().contains(mousePos))
                {
                    m_selectedOption = 100;
                    audio.playSound("menu_select");
                }
            }
        }

        if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>())
        {
            if (m_isDraggingSlider && m_volumeSliderHandleSprite)
            {
                const sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(mouseMoved->position.x, mouseMoved->position.y));
                float clampedX = std::max(m_sliderTrackLeft, std::min(mousePos.x, m_sliderTrackLeft + m_sliderTrackWidth));

                // Update volume based on slider position
                m_volumeLevel = ((clampedX - m_sliderTrackLeft) / m_sliderTrackWidth) * 100.f;
                m_volumeSliderHandleSprite->setPosition({clampedX, 290.f});
                updateVolumeDisplay(&audio);
            }
        }

        if (const auto *mouseButton = event->getIf<sf::Event::MouseButtonReleased>())
        {
            if (mouseButton->button == sf::Mouse::Button::Left)
            {
                m_isDraggingSlider = false;
            }
        }
    }
}

void OptionsMenu::toggleFullscreen()
{
    m_isFullscreen = !m_isFullscreen;
    applyWindowSettings();
}

void OptionsMenu::applyWindowSettings()
{
    const sf::VideoMode videoMode({1920, 1080});
    m_window.close();
    if (m_isFullscreen)
        m_window.create(videoMode, GAME_TITLE, sf::Style::Default, sf::State::Fullscreen);
    else
        m_window.create(videoMode, GAME_TITLE, sf::Style::Default, sf::State::Windowed);

    m_window.setFramerateLimit(60);

    // Re-apply view
    const float baseW = static_cast<float>(BASE_WINDOW_WIDTH);
    const float baseH = static_cast<float>(BASE_WINDOW_HEIGHT);
    sf::View view(sf::FloatRect({0.f, 0.f}, {baseW, baseH}));
    view.setCenter({baseW / 2.f, baseH / 2.f});
    m_window.setView(view);
}

void OptionsMenu::updateVolumeDisplay(AudioManager *audio)
{
    // Update global config
    AUDIO_VOLUME = m_volumeLevel;

    // Update volume value text display
    if (m_volumeValueText)
    {
        m_volumeValueText->setString(std::to_string(static_cast<int>(m_volumeLevel)));
    }

    // Update real audio volume if manager is provided
    if (audio)
    {
        audio->setMusicVolume(m_volumeLevel);
    }
}

void OptionsMenu::render()
{
    updateSelection();

    m_window.clear(sf::Color::Black);

    if (m_backgroundSprite)
    {
        const sf::Vector2u texSize = m_backgroundTexture.getSize();
        const sf::Vector2u winSize = m_window.getSize();
        if (texSize.x > 0 && texSize.y > 0)
        {
            m_backgroundSprite->setScale(sf::Vector2f{
                static_cast<float>(winSize.x) / texSize.x,
                static_cast<float>(winSize.y) / texSize.y});
        }
        m_window.draw(*m_backgroundSprite);
    }

    // Dim overlay similar to pause menu
    const sf::Vector2u winSize = m_window.getSize();
    sf::RectangleShape overlay({static_cast<float>(winSize.x), static_cast<float>(winSize.y)});
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    m_window.draw(overlay);

    // Draw all sprites
    if (m_optionsTitleSprite)
        m_window.draw(*m_optionsTitleSprite);
    if (m_soundSprite)
        m_window.draw(*m_soundSprite);
    if (m_volumeSliderBackgroundSprite)
        m_window.draw(*m_volumeSliderBackgroundSprite);

    // Draw white line on top of button for volume slider
    sf::RectangleShape sliderLine({m_sliderTrackWidth, 2.f});
    sliderLine.setPosition({m_sliderTrackLeft, 282.f});
    sliderLine.setFillColor(sf::Color::White);
    m_window.draw(sliderLine);

    if (m_volumeSliderHandleSprite)
        m_window.draw(*m_volumeSliderHandleSprite);
    if (m_volumeValueText)
        m_window.draw(*m_volumeValueText);
    if (m_fullscreenButtonSprite)
        m_window.draw(*m_fullscreenButtonSprite);
    if (m_isFullscreen && m_onSprite)
        m_window.draw(*m_onSprite);
    else if (!m_isFullscreen && m_offSprite)
        m_window.draw(*m_offSprite);
    if (m_backButtonSprite)
        m_window.draw(*m_backButtonSprite);
}