#include "OptionsMenu.h"
#include "../Config.h"
#include <iostream>
#include <sstream>

OptionsMenu::OptionsMenu(sf::RenderWindow &window)
    : m_window(window), m_selectedOption(0), m_isResolutionDropdownOpen(false),
      m_selectedResolutionIndex(2), m_isDraggingSlider(false),
      m_volumeLevel(50.f), m_isFullscreen(false)
{
    if (!m_font.openFromFile(FONT_PATH))
    {
        std::cerr << "Failed to load font" << std::endl;
    }

    // Initialize resolutions
    m_resolutions = {
        {1280, 720},
        {1600, 900},
        {1920, 1080},
        {2560, 1440}};

    setupUI();
}

void OptionsMenu::setupUI()
{
    float startX = 200.f;
    float startY = 150.f;
    float lineHeight = 80.f;

    // Title
    m_titleText = std::make_unique<sf::Text>(m_font, "OPTIONS", 60);
    m_titleText->setFillColor(sf::Color::White);
    m_titleText->setPosition({startX, 50.f});

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

        // Position text inside the button
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
    // Reset all colors
    if (m_volumeValue)
        m_volumeValue->setFillColor(m_textColor);
    m_fullscreenButton.setFillColor(m_buttonColor);
    m_resolutionButton.setFillColor(m_buttonColor);
    m_backButton.setFillColor(m_buttonColor);

    // Reset resolution options
    for (auto &button : m_resolutionOptionButtons)
        button.setFillColor(m_buttonColor);

    // Highlight selected option
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

void OptionsMenu::handleInput()
{
    while (const std::optional event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            m_window.close();

        if (const auto *key = event->getIf<sf::Event::KeyPressed>())
        {
            if (m_isResolutionDropdownOpen)
            {
                // Handle resolution dropdown navigation
                if (key->code == sf::Keyboard::Key::Up)
                {
                    m_selectedOption = std::max(3, m_selectedOption - 1);
                }
                else if (key->code == sf::Keyboard::Key::Down)
                {
                    m_selectedOption = std::min(static_cast<int>(3 + m_resolutions.size() - 1), m_selectedOption + 1);
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
                // Handle main menu navigation
                if (key->code == sf::Keyboard::Key::Up)
                {
                    m_selectedOption = (m_selectedOption - 1 + 3) % 3;
                }
                else if (key->code == sf::Keyboard::Key::Down)
                {
                    m_selectedOption = (m_selectedOption + 1) % 3;
                }
                else if (key->code == sf::Keyboard::Key::Left && m_selectedOption == 0)
                {
                    m_volumeLevel = std::max(0.f, m_volumeLevel - 5.f);
                    updateVolumeDisplay();
                }
                else if (key->code == sf::Keyboard::Key::Right && m_selectedOption == 0)
                {
                    m_volumeLevel = std::min(100.f, m_volumeLevel + 5.f);
                    updateVolumeDisplay();
                }
                else if (key->code == sf::Keyboard::Key::Enter || key->code == sf::Keyboard::Key::Space)
                {
                    if (m_selectedOption == 1)
                    {
                        toggleFullscreen();
                    }
                    else if (m_selectedOption == 2)
                    {
                        m_isResolutionDropdownOpen = true;
                        m_selectedOption = 3;
                    }
                }
                else if (key->code == sf::Keyboard::Key::Escape)
                {
                    m_selectedOption = 100;
                }
            }
        }

        // Handle mouse movement
        if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>())
        {
            const sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(mouseMoved->position.x, mouseMoved->position.y));

            // Handle slider dragging
            if (m_isDraggingSlider)
            {
                float sliderLeft = m_volumeSlider.getPosition().x;
                float sliderRight = sliderLeft + m_volumeSlider.getSize().x;
                float clampedX = std::max(sliderLeft, std::min(mousePos.x, sliderRight));

                m_volumeLevel = ((clampedX - sliderLeft) / m_volumeSlider.getSize().x) * 100.f;
                updateVolumeDisplay();
            }

            // Check if hovering over volume slider
            if (m_volumeSlider.getGlobalBounds().contains(mousePos) || m_volumeHandle.getGlobalBounds().contains(mousePos))
            {
                m_selectedOption = 0;
            }
            // Check if hovering over fullscreen button
            else if (m_fullscreenButton.getGlobalBounds().contains(mousePos))
            {
                m_selectedOption = 1;
            }
            // Check if hovering over resolution button
            else if (m_resolutionButton.getGlobalBounds().contains(mousePos))
            {
                m_selectedOption = 2;
            }
            // Check resolution dropdown options
            else if (m_isResolutionDropdownOpen)
            {
                bool foundOption = false;
                for (size_t i = 0; i < m_resolutionOptionButtons.size(); ++i)
                {
                    if (m_resolutionOptionButtons[i].getGlobalBounds().contains(mousePos))
                    {
                        m_selectedOption = 3 + static_cast<int>(i);
                        foundOption = true;
                        break;
                    }
                }
            }
        }

        // Handle mouse clicks
        if (const auto *mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButton->button == sf::Mouse::Button::Left)
            {
                const sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(mouseButton->position.x, mouseButton->position.y));

                if (m_volumeHandle.getGlobalBounds().contains(mousePos) || m_volumeSlider.getGlobalBounds().contains(mousePos))
                {
                    m_isDraggingSlider = true;
                    m_selectedOption = 0;
                    float sliderLeft = m_volumeSlider.getPosition().x;
                    float sliderRight = sliderLeft + m_volumeSlider.getSize().x;
                    float clampedX = std::max(sliderLeft, std::min(mousePos.x, sliderRight));
                    m_volumeLevel = ((clampedX - sliderLeft) / m_volumeSlider.getSize().x) * 100.f;
                    updateVolumeDisplay();
                }
                else if (m_fullscreenButton.getGlobalBounds().contains(mousePos))
                {
                    m_selectedOption = 1;
                    toggleFullscreen();
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
                            break;
                        }
                    }
                }
                else if (m_backButton.getGlobalBounds().contains(mousePos))
                {
                    m_selectedOption = 100;
                }
            }
        }

        // Handle mouse button release
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
    WINDOW_WIDTH = selected.x;
    WINDOW_HEIGHT = selected.y;

    const sf::VideoMode videoMode(selected);
    m_window.close();
    if (m_isFullscreen)
    {
        m_window.create(videoMode, GAME_TITLE, sf::Style::Default, sf::State::Fullscreen);
    }
    else
    {
        m_window.create(videoMode, GAME_TITLE, sf::Style::Default, sf::State::Windowed);
    }

    m_window.setFramerateLimit(60);

    // Use a virtual view based on the base resolution so smaller windows zoom out instead of clipping.
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

void OptionsMenu::updateVolumeDisplay()
{
    m_volumeValue->setString(std::to_string(static_cast<int>(m_volumeLevel)) + "%");
    float sliderPos = (m_volumeLevel / 100.f) * 200.f;
    m_volumeHandle.setPosition({m_volumeSlider.getPosition().x + sliderPos - 7.5f, m_volumeHandle.getPosition().y});

    // Update the global audio volume in Config
    AUDIO_VOLUME = m_volumeLevel;
}

void OptionsMenu::render()
{
    updateSelection();

    m_window.clear(sf::Color::Black);

    // Render title
    if (m_titleText)
        m_window.draw(*m_titleText);

    // Render volume controls
    if (m_volumeLabel)
        m_window.draw(*m_volumeLabel);
    m_window.draw(m_volumeSlider);
    m_window.draw(m_volumeHandle);
    if (m_volumeValue)
        m_window.draw(*m_volumeValue);

    // Render fullscreen controls
    if (m_fullscreenLabel)
        m_window.draw(*m_fullscreenLabel);
    m_window.draw(m_fullscreenButton);
    if (m_fullscreenValue)
        m_window.draw(*m_fullscreenValue);

    // Render resolution controls
    if (m_resolutionLabel)
        m_window.draw(*m_resolutionLabel);
    m_window.draw(m_resolutionButton);
    if (m_resolutionValue)
        m_window.draw(*m_resolutionValue);

    // Render resolution dropdown if open
    if (m_isResolutionDropdownOpen)
    {
        for (size_t i = 0; i < m_resolutionOptionButtons.size(); ++i)
        {
            m_window.draw(m_resolutionOptionButtons[i]);
            if (m_resolutionOptions[i])
                m_window.draw(*m_resolutionOptions[i]);
        }
    }

    // Render back button
    m_window.draw(m_backButton);
    if (m_backButtonText)
        m_window.draw(*m_backButtonText);

    m_window.display();
}
