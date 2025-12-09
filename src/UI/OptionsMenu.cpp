#include "OptionsMenu.h"
#include "../Core/Config.h"
#include <iostream>
#include <sstream>
#include <algorithm> // For std::max and std::min

OptionsMenu::OptionsMenu(sf::RenderWindow &window)
    : m_window(window), m_selectedOption(0), m_isResolutionDropdownOpen(false),
      m_selectedResolutionIndex(2), m_isDraggingSlider(false),
      m_volumeLevel(50.f), m_isFullscreen(false)
{
    // Try to load font from Config path, fallback to system path
    if (!m_font.openFromFile(FONT_PATH))
    {
        if (!m_font.openFromFile("C:\\Windows\\Fonts\\arial.ttf"))
            std::cerr << "Failed to load font" << std::endl;
    }

    // Initialize resolutions
    m_resolutions = {
        {1280, 720},
        {1600, 900},
        {1920, 1080},
        {2560, 1440}};

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
    float startX = 200.f;
    float startY = 460.f; // move content further down
    float lineHeight = 80.f;

    // Title
    m_titleText = std::make_unique<sf::Text>(m_font, "OPTIONS", 60);
    m_titleText->setFillColor(sf::Color::White);
    m_titleText->setPosition({startX, 340.f});

    // Volume Label
    m_volumeLabel = std::make_unique<sf::Text>(m_font, "Volume:", 40);
    m_volumeLabel->setFillColor(m_textColor);
    m_volumeLabel->setPosition({startX, startY});

    // Volume Value
    m_volumeValue = std::make_unique<sf::Text>(m_font, "50%", 40);
    m_volumeValue->setFillColor(m_textColor);
    m_volumeValue->setPosition({startX + 400.f, startY});

    // Volume Slider
    m_volumeSlider.setSize({200.f, 20.f});
    m_volumeSlider.setPosition({startX + 200.f, startY + 10.f});
    m_volumeSlider.setFillColor(sf::Color::White);

    m_volumeHandle.setSize({15.f, 40.f});
    m_volumeHandle.setPosition({startX + 200.f + (m_volumeLevel / 100.f) * 200.f - 7.5f, startY});
    m_volumeHandle.setFillColor(m_selectedColor);

    // Fullscreen Label
    float fullscreenY = startY + lineHeight;
    m_fullscreenLabel = std::make_unique<sf::Text>(m_font, "Fullscreen:", 40);
    m_fullscreenLabel->setFillColor(m_textColor);
    m_fullscreenLabel->setPosition({startX, fullscreenY});

    m_fullscreenButton.setSize({80.f, 50.f});
    m_fullscreenButton.setPosition({startX + 400.f, fullscreenY + 5.f});
    m_fullscreenButton.setFillColor(m_buttonColor);

    m_fullscreenValue = std::make_unique<sf::Text>(m_font, "OFF", 35);
    m_fullscreenValue->setFillColor(sf::Color::Black);
    m_fullscreenValue->setPosition({m_fullscreenButton.getPosition() + sf::Vector2f{10.f, 5.f}});

    // Resolution Label
    float resolutionY = fullscreenY + lineHeight;
    m_resolutionLabel = std::make_unique<sf::Text>(m_font, "Resolution:", 40);
    m_resolutionLabel->setFillColor(m_textColor);
    m_resolutionLabel->setPosition({startX, resolutionY});

    m_resolutionButton.setSize({200.f, 50.f});
    m_resolutionButton.setPosition({startX + 400.f, resolutionY + 5.f});
    m_resolutionButton.setFillColor(m_buttonColor);

    m_resolutionValue = std::make_unique<sf::Text>(m_font, resolutionToString(m_resolutions[m_selectedResolutionIndex]), 35);
    m_resolutionValue->setFillColor(sf::Color::Black);
    m_resolutionValue->setPosition({m_resolutionButton.getPosition() + sf::Vector2f{20.f, 5.f}});

    // Resolution dropdown options
    for (size_t i = 0; i < m_resolutions.size(); ++i)
    {
        auto optionText = std::make_unique<sf::Text>(m_font, resolutionToString(m_resolutions[i]), 30);
        optionText->setFillColor(sf::Color::Black);

        sf::RectangleShape optionButton({180.f, 40.f});
        optionButton.setPosition({startX + 420.f, resolutionY + 70.f + static_cast<float>(i) * 50.f});
        optionButton.setFillColor(m_buttonColor);

        optionText->setPosition({optionButton.getPosition() + sf::Vector2f{15.f, 3.f}});

        m_resolutionOptions.push_back(std::move(optionText));
        m_resolutionOptionButtons.push_back(optionButton);
    }

    // Back button
    m_backButton.setSize({200.f, 60.f});
    m_backButton.setPosition({startX, resolutionY + lineHeight + 50.f});
    m_backButton.setFillColor(m_buttonColor);

    m_backButtonText = std::make_unique<sf::Text>(m_font, "BACK", 40);
    m_backButtonText->setFillColor(sf::Color::Black);
    m_backButtonText->setPosition({m_backButton.getPosition() + sf::Vector2f{30.f, 8.f}});
}

void OptionsMenu::updateSelection()
{
    if (m_volumeValue)
        m_volumeValue->setFillColor(m_textColor);
    m_fullscreenButton.setFillColor(m_buttonColor);
    m_resolutionButton.setFillColor(m_buttonColor);
    m_backButton.setFillColor(m_buttonColor);

    for (auto &button : m_resolutionOptionButtons)
        button.setFillColor(m_buttonColor);

    if (m_selectedOption == 0)
    {
        if (m_volumeValue)
            m_volumeValue->setFillColor(m_selectedColor);
    }
    else if (m_selectedOption == 1)
    {
        m_fullscreenButton.setFillColor(m_selectedColor);
    }
    else if (m_selectedOption == 2)
    {
        m_resolutionButton.setFillColor(m_selectedColor);
    }
    else if (m_isResolutionDropdownOpen && m_selectedOption >= 3 && m_selectedOption < static_cast<int>(3 + m_resolutions.size()))
    {
        int optionIndex = m_selectedOption - 3;
        if (optionIndex >= 0 && optionIndex < static_cast<int>(m_resolutionOptionButtons.size()))
            m_resolutionOptionButtons[optionIndex].setFillColor(m_selectedColor);
    }
    else if (m_selectedOption == 100)
    {
        m_backButton.setFillColor(m_selectedColor);
    }
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
            if (m_isResolutionDropdownOpen)
            {
                if (key->code == sf::Keyboard::Key::Up)
                {
                    m_selectedOption = std::max(3, m_selectedOption - 1);
                    audio.playSound("menu_move");
                }
                else if (key->code == sf::Keyboard::Key::Down)
                {
                    m_selectedOption = std::min(static_cast<int>(3 + m_resolutions.size() - 1), m_selectedOption + 1);
                    audio.playSound("menu_move");
                }
                else if (key->code == sf::Keyboard::Key::Enter || key->code == sf::Keyboard::Key::Space)
                {
                    int optionIndex = m_selectedOption - 3;
                    if (optionIndex >= 0 && optionIndex < static_cast<int>(m_resolutions.size()))
                    {
                        m_selectedResolutionIndex = optionIndex;
                        updateResolutionDisplay();
                        m_isResolutionDropdownOpen = false;
                        m_selectedOption = 2;
                        audio.playSound("menu_select");
                    }
                }
                else if (key->code == sf::Keyboard::Key::Escape)
                {
                    m_isResolutionDropdownOpen = false;
                    m_selectedOption = 2;
                }
            }
            else
            {
                // Main Options Navigation
                if (key->code == sf::Keyboard::Key::Up)
                {
                    // Logic to cycle: 0(Vol) -> 1(Full) -> 2(Res) -> 100(Back)
                    if (m_selectedOption == 0)
                        m_selectedOption = 100;
                    else if (m_selectedOption == 100)
                        m_selectedOption = 2;
                    else
                        m_selectedOption--;

                    audio.playSound("menu_move");
                }
                else if (key->code == sf::Keyboard::Key::Down)
                {
                    if (m_selectedOption == 2)
                        m_selectedOption = 100;
                    else if (m_selectedOption == 100)
                        m_selectedOption = 0;
                    else
                        m_selectedOption++;

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
                    else if (m_selectedOption == 2)
                    {
                        m_isResolutionDropdownOpen = true;
                        m_selectedOption = 3;
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
        }

        // Mouse Handling
        if (const auto *mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButton->button == sf::Mouse::Button::Left)
            {
                const sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(mouseButton->position.x, mouseButton->position.y));

                if (m_volumeSlider.getGlobalBounds().contains(mousePos) || m_volumeHandle.getGlobalBounds().contains(mousePos))
                {
                    m_isDraggingSlider = true;
                    m_selectedOption = 0;
                    // Calculate immediate click position
                    float sliderLeft = m_volumeSlider.getPosition().x;
                    float clampedX = std::max(sliderLeft, std::min(mousePos.x, sliderLeft + m_volumeSlider.getSize().x));
                    m_volumeLevel = ((clampedX - sliderLeft) / m_volumeSlider.getSize().x) * 100.f;
                    updateVolumeDisplay(&audio);
                }
                else if (m_fullscreenButton.getGlobalBounds().contains(mousePos))
                {
                    m_selectedOption = 1;
                    toggleFullscreen();
                    audio.playSound("menu_select");
                }
                else if (m_resolutionButton.getGlobalBounds().contains(mousePos))
                {
                    m_selectedOption = 2;
                    m_isResolutionDropdownOpen = !m_isResolutionDropdownOpen;
                    if (m_isResolutionDropdownOpen)
                        m_selectedOption = 3;
                }
                else if (m_isResolutionDropdownOpen)
                {
                    for (size_t i = 0; i < m_resolutionOptionButtons.size(); ++i)
                    {
                        if (m_resolutionOptionButtons[i].getGlobalBounds().contains(mousePos))
                        {
                            m_selectedResolutionIndex = i;
                            updateResolutionDisplay();
                            m_isResolutionDropdownOpen = false;
                            m_selectedOption = 2;
                            audio.playSound("menu_select");
                            break;
                        }
                    }
                }
                else if (m_backButton.getGlobalBounds().contains(mousePos))
                {
                    m_selectedOption = 100;
                    audio.playSound("menu_select");
                }
            }
        }

        if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>())
        {
            if (m_isDraggingSlider)
            {
                const sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(mouseMoved->position.x, mouseMoved->position.y));
                float sliderLeft = m_volumeSlider.getPosition().x;
                float clampedX = std::max(sliderLeft, std::min(mousePos.x, sliderLeft + m_volumeSlider.getSize().x));

                m_volumeLevel = ((clampedX - sliderLeft) / m_volumeSlider.getSize().x) * 100.f;
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
    m_fullscreenValue->setString(m_isFullscreen ? "ON" : "OFF");
    applyWindowSettings();
}

void OptionsMenu::updateResolutionDisplay()
{
    m_resolutionValue->setString(resolutionToString(m_resolutions[m_selectedResolutionIndex]));
    applyWindowSettings();
}

void OptionsMenu::applyWindowSettings()
{
    const sf::Vector2u selected = m_resolutions[m_selectedResolutionIndex];
    // Update global config variables
    WINDOW_WIDTH = selected.x;
    WINDOW_HEIGHT = selected.y;

    const sf::VideoMode videoMode(selected);
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

std::string OptionsMenu::resolutionToString(sf::Vector2u res) const
{
    return std::to_string(res.x) + "x" + std::to_string(res.y);
}

void OptionsMenu::updateVolumeDisplay(AudioManager *audio)
{
    m_volumeValue->setString(std::to_string(static_cast<int>(m_volumeLevel)) + "%");
    float sliderPos = (m_volumeLevel / 100.f) * 200.f;
    m_volumeHandle.setPosition({m_volumeSlider.getPosition().x + sliderPos - 7.5f, m_volumeHandle.getPosition().y});

    // Update global config
    AUDIO_VOLUME = m_volumeLevel;

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

    if (m_titleText)
        m_window.draw(*m_titleText);

    if (m_volumeLabel)
        m_window.draw(*m_volumeLabel);
    m_window.draw(m_volumeSlider);
    m_window.draw(m_volumeHandle);
    if (m_volumeValue)
        m_window.draw(*m_volumeValue);

    if (m_fullscreenLabel)
        m_window.draw(*m_fullscreenLabel);
    m_window.draw(m_fullscreenButton);
    if (m_fullscreenValue)
        m_window.draw(*m_fullscreenValue);

    if (m_resolutionLabel)
        m_window.draw(*m_resolutionLabel);
    m_window.draw(m_resolutionButton);
    if (m_resolutionValue)
        m_window.draw(*m_resolutionValue);

    if (m_isResolutionDropdownOpen)
    {
        for (size_t i = 0; i < m_resolutionOptionButtons.size(); ++i)
        {
            m_window.draw(m_resolutionOptionButtons[i]);
            if (m_resolutionOptions[i])
                m_window.draw(*m_resolutionOptions[i]);
        }
    }

    m_window.draw(m_backButton);
    if (m_backButtonText)
        m_window.draw(*m_backButtonText);
}