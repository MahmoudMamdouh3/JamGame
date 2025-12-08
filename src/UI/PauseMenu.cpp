#include "PauseMenu.h"
#include "OptionsMenu.h"
#include "../Config.h"
#include <iostream>

PauseMenu::PauseMenu(sf::RenderWindow &window)
    : m_window(window), m_selectedOption(0), m_optionsOpen(false), m_selectionMade(false), m_optionsMenuPtr(nullptr)
{
    if (!m_font.openFromFile(FONT_PATH))
    {
        std::cerr << "Failed to load font" << std::endl;
    }

    // Setup semi-transparent overlay
    m_overlay.setSize({static_cast<float>(BASE_WINDOW_WIDTH), static_cast<float>(BASE_WINDOW_HEIGHT)});
    m_overlay.setPosition({0.f, 0.f});
    m_overlay.setFillColor(sf::Color(0, 0, 0, 150));

    // Create the options menu
    m_optionsMenuPtr = new OptionsMenu(window);

    setupUI();
}

void PauseMenu::setupUI()
{
    setupButtons();
    setupText();
    updateLayout();
}

void PauseMenu::setupButtons()
{
    float buttonWidth = 300.f;
    float buttonHeight = 60.f;
    float centerX = static_cast<float>(BASE_WINDOW_WIDTH) / 2.f - buttonWidth / 2.f;
    float startY = static_cast<float>(BASE_WINDOW_HEIGHT) / 2.f - 150.f;
    float spacing = 90.f;

    // Resume Button
    m_resumeButton.setSize({buttonWidth, buttonHeight});
    m_resumeButton.setPosition({centerX, startY});
    m_resumeButton.setFillColor(m_selectedColor);

    // Restart Button
    m_restartButton.setSize({buttonWidth, buttonHeight});
    m_restartButton.setPosition({centerX, startY + spacing});
    m_restartButton.setFillColor(m_buttonColor);

    // Options Button
    m_optionsButton.setSize({buttonWidth, buttonHeight});
    m_optionsButton.setPosition({centerX, startY + spacing * 2});
    m_optionsButton.setFillColor(m_buttonColor);

    // Main Menu Button
    m_mainMenuButton.setSize({buttonWidth, buttonHeight});
    m_mainMenuButton.setPosition({centerX, startY + spacing * 3});
    m_mainMenuButton.setFillColor(m_buttonColor);
}

void PauseMenu::setupText()
{
    // Title
    m_titleText = std::make_unique<sf::Text>(m_font, "PAUSED", 60);
    m_titleText->setFillColor(sf::Color::White);
    m_titleText->setPosition({static_cast<float>(BASE_WINDOW_WIDTH) / 2.f - 120.f, 100.f});

    // Resume Text
    m_resumeText = std::make_unique<sf::Text>(m_font, "Resume", 40);
    m_resumeText->setFillColor(m_textColor);

    // Restart Text
    m_restartText = std::make_unique<sf::Text>(m_font, "Restart", 40);
    m_restartText->setFillColor(m_textColor);

    // Options Text
    m_optionsText = std::make_unique<sf::Text>(m_font, "Options", 40);
    m_optionsText->setFillColor(m_textColor);

    // Main Menu Text
    m_mainMenuText = std::make_unique<sf::Text>(m_font, "Main Menu", 40);
    m_mainMenuText->setFillColor(m_textColor);

    updateTextPositions();
}

void PauseMenu::updateTextPositions()
{
    if (m_resumeText)
        m_resumeText->setPosition(m_resumeButton.getPosition() + sf::Vector2f{50.f, 10.f});
    if (m_restartText)
        m_restartText->setPosition(m_restartButton.getPosition() + sf::Vector2f{50.f, 10.f});
    if (m_optionsText)
        m_optionsText->setPosition(m_optionsButton.getPosition() + sf::Vector2f{50.f, 10.f});
    if (m_mainMenuText)
        m_mainMenuText->setPosition(m_mainMenuButton.getPosition() + sf::Vector2f{30.f, 10.f});
}

void PauseMenu::updateLayout()
{
    const sf::Vector2u currentSize = m_window.getSize();
    if (currentSize == m_lastWindowSize || currentSize.x == 0 || currentSize.y == 0)
        return;

    m_lastWindowSize = currentSize;

    float buttonWidth = 300.f;
    float buttonHeight = 60.f;
    float centerX = static_cast<float>(BASE_WINDOW_WIDTH) / 2.f - buttonWidth / 2.f;
    float startY = static_cast<float>(BASE_WINDOW_HEIGHT) / 2.f - 150.f;
    float spacing = 90.f;

    m_resumeButton.setPosition({centerX, startY});
    m_restartButton.setPosition({centerX, startY + spacing});
    m_optionsButton.setPosition({centerX, startY + spacing * 2});
    m_mainMenuButton.setPosition({centerX, startY + spacing * 3});

    updateTextPositions();
}

void PauseMenu::updateButtonSelection()
{
    m_resumeButton.setFillColor(m_buttonColor);
    m_restartButton.setFillColor(m_buttonColor);
    m_optionsButton.setFillColor(m_buttonColor);
    m_mainMenuButton.setFillColor(m_buttonColor);

    if (m_selectedOption == 0)
        m_resumeButton.setFillColor(m_selectedColor);
    else if (m_selectedOption == 1)
        m_restartButton.setFillColor(m_selectedColor);
    else if (m_selectedOption == 2)
        m_optionsButton.setFillColor(m_selectedColor);
    else if (m_selectedOption == 3)
        m_mainMenuButton.setFillColor(m_selectedColor);
}

void PauseMenu::handleInput()
{
    if (m_optionsOpen)
    {
        // Handle options menu input
        m_optionsMenuPtr->handleInput();

        if (m_optionsMenuPtr->isBack())
        {
            m_optionsOpen = false;
            m_optionsMenuPtr->resetSelection();
        }
    }
    else
    {
        while (const std::optional event = m_window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                m_window.close();

            if (const auto *key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Escape)
                {
                    // Escape resumes the game
                    m_selectedOption = 0; // Resume
                    m_selectionMade = true;
                }
                else if (key->code == sf::Keyboard::Key::Up)
                {
                    m_selectedOption = (m_selectedOption - 1 + 4) % 4;
                }
                else if (key->code == sf::Keyboard::Key::Down)
                {
                    m_selectedOption = (m_selectedOption + 1) % 4;
                }
                else if (key->code == sf::Keyboard::Key::Enter || key->code == sf::Keyboard::Key::Space)
                {
                    // Selection will be handled by Game class
                    m_selectionMade = true;
                }
            }

            // Handle mouse movement
            if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                const sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(mouseMoved->position.x, mouseMoved->position.y));

                if (isMouseOverButton(m_resumeButton, mousePos))
                    m_selectedOption = 0;
                else if (isMouseOverButton(m_restartButton, mousePos))
                    m_selectedOption = 1;
                else if (isMouseOverButton(m_optionsButton, mousePos))
                    m_selectedOption = 2;
                else if (isMouseOverButton(m_mainMenuButton, mousePos))
                    m_selectedOption = 3;
            }

            // Handle mouse clicks
            if (const auto *mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButton->button == sf::Mouse::Button::Left)
                {
                    const sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(mouseButton->position.x, mouseButton->position.y));
                    checkMouseClick(mousePos);
                }
            }
        }
    }
}

void PauseMenu::checkMouseClick(const sf::Vector2f &mousePos)
{
    if (m_resumeButton.getGlobalBounds().contains(mousePos))
    {
        m_selectedOption = 0;
        m_selectionMade = true;
    }
    else if (m_restartButton.getGlobalBounds().contains(mousePos))
    {
        m_selectedOption = 1;
        m_selectionMade = true;
    }
    else if (m_optionsButton.getGlobalBounds().contains(mousePos))
    {
        m_selectedOption = 2;
        m_selectionMade = true;
    }
    else if (m_mainMenuButton.getGlobalBounds().contains(mousePos))
    {
        m_selectedOption = 3;
        m_selectionMade = true;
    }
}

bool PauseMenu::isMouseOverButton(const sf::RectangleShape &button, const sf::Vector2f &mousePos) const
{
    return button.getGlobalBounds().contains(mousePos);
}

void PauseMenu::render()
{
    if (m_optionsOpen)
    {
        // Render options menu
        m_optionsMenuPtr->render();
    }
    else
    {
        updateLayout();
        updateButtonSelection();

        // Draw semi-transparent overlay
        m_window.draw(m_overlay);

        // Draw buttons
        m_window.draw(m_resumeButton);
        m_window.draw(m_restartButton);
        m_window.draw(m_optionsButton);
        m_window.draw(m_mainMenuButton);

        // Draw text
        if (m_titleText)
            m_window.draw(*m_titleText);
        if (m_resumeText)
            m_window.draw(*m_resumeText);
        if (m_restartText)
            m_window.draw(*m_restartText);
        if (m_optionsText)
            m_window.draw(*m_optionsText);
        if (m_mainMenuText)
            m_window.draw(*m_mainMenuText);
    }
}
