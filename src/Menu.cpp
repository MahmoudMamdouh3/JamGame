#include "Menu.h"
#include "Config.h"
#include <iostream>

Menu::Menu(sf::RenderWindow &window)
    : m_window(window), m_currentState(MenuState::Main), m_selectedOption(0), m_selectionMade(false)
{
    setupButtons();
    setupText();
}

void Menu::setupButtons()
{
    // Button dimensions
    float buttonWidth = 300.f;
    float buttonHeight = 80.f;
    float leftMargin = 100.f;
    float startY = WINDOW_HEIGHT / 2 - 150.f;
    float spacing = 120.f;

    // Start Button
    m_startButton.setSize({buttonWidth, buttonHeight});
    m_startButton.setPosition({leftMargin, startY});
    m_startButton.setFillColor(m_selectedColor);

    // Options Button
    m_optionsButton.setSize({buttonWidth, buttonHeight});
    m_optionsButton.setPosition({leftMargin, startY + spacing});
    m_optionsButton.setFillColor(m_buttonColor);

    // Exit Button
    m_exitButton.setSize({buttonWidth, buttonHeight});
    m_exitButton.setPosition({leftMargin, startY + spacing * 2});
    m_exitButton.setFillColor(m_buttonColor);
}

void Menu::setupText()
{
    // Load font - using a system font path
    if (!m_font.openFromFile("C:\\Windows\\Fonts\\arial.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }

    // Start Text
    m_startText = std::make_unique<sf::Text>(m_font, "START", 40);
    m_startText->setFillColor(m_textColor);
    m_startText->setPosition(m_startButton.getPosition() + sf::Vector2f{60.f, 15.f});

    // Options Text
    m_optionsText = std::make_unique<sf::Text>(m_font, "OPTIONS", 40);
    m_optionsText->setFillColor(m_textColor);
    m_optionsText->setPosition(m_optionsButton.getPosition() + sf::Vector2f{35.f, 15.f});

    // Exit Text
    m_exitText = std::make_unique<sf::Text>(m_font, "EXIT", 40);
    m_exitText->setFillColor(m_textColor);
    m_exitText->setPosition(m_exitButton.getPosition() + sf::Vector2f{80.f, 15.f});

    // Game Name Text (on the right side)
    m_gameNameText = std::make_unique<sf::Text>(m_font, "Isometric\nFullscreen\nCity", 60);
    m_gameNameText->setFillColor(sf::Color::White);
    m_gameNameText->setPosition({WINDOW_WIDTH - 500.f, WINDOW_HEIGHT / 2 - 100.f});
}

void Menu::handleInput()
{
    while (const std::optional event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            m_window.close();

        if (const auto *key = event->getIf<sf::Event::KeyPressed>())
        {
            // Navigate with arrow keys
            if (key->code == sf::Keyboard::Key::Up)
            {
                m_selectedOption = (m_selectedOption - 1 + 3) % 3;
            }
            else if (key->code == sf::Keyboard::Key::Down)
            {
                m_selectedOption = (m_selectedOption + 1) % 3;
            }
            // Select with Enter or Space
            else if (key->code == sf::Keyboard::Key::Enter || key->code == sf::Keyboard::Key::Space)
            {
                m_selectionMade = true;
                if (m_selectedOption == 0)
                    m_currentState = MenuState::Main; // Start game
                else if (m_selectedOption == 1)
                    m_currentState = MenuState::Options; // Options
                else if (m_selectedOption == 2)
                    m_currentState = MenuState::Exit; // Exit
            }
            // Escape to exit
            else if (key->code == sf::Keyboard::Key::Escape)
            {
                m_window.close();
            }
        }

        // Handle mouse movement for button hover
        if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>())
        {
            sf::Vector2f mousePos(static_cast<float>(mouseMoved->position.x), static_cast<float>(mouseMoved->position.y));

            if (isMouseOverButton(m_startButton))
                m_selectedOption = 0;
            else if (isMouseOverButton(m_optionsButton))
                m_selectedOption = 1;
            else if (isMouseOverButton(m_exitButton))
                m_selectedOption = 2;
        }

        // Handle mouse clicks
        if (const auto *mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButton->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(mouseButton->position.x), static_cast<float>(mouseButton->position.y));
                checkMouseClick(mousePos);
            }
        }
    }
}

void Menu::updateButtonSelection()
{
    // Reset all buttons to normal color
    m_startButton.setFillColor(m_buttonColor);
    m_optionsButton.setFillColor(m_buttonColor);
    m_exitButton.setFillColor(m_buttonColor);

    // Highlight selected button
    if (m_selectedOption == 0)
        m_startButton.setFillColor(m_selectedColor);
    else if (m_selectedOption == 1)
        m_optionsButton.setFillColor(m_selectedColor);
    else if (m_selectedOption == 2)
        m_exitButton.setFillColor(m_selectedColor);
}

void Menu::render()
{
    updateButtonSelection();

    m_window.clear(sf::Color::Black);

    // Render buttons
    m_window.draw(m_startButton);
    m_window.draw(m_optionsButton);
    m_window.draw(m_exitButton);

    // Render text
    if (m_startText)
        m_window.draw(*m_startText);
    if (m_optionsText)
        m_window.draw(*m_optionsText);
    if (m_exitText)
        m_window.draw(*m_exitText);
    if (m_gameNameText)
        m_window.draw(*m_gameNameText);

    m_window.display();
}

bool Menu::isMouseOverButton(const sf::RectangleShape &button) const
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
    sf::Vector2f buttonPos = button.getPosition();
    sf::Vector2f buttonSize = button.getSize();

    return mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + buttonSize.x &&
           mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + buttonSize.y;
}

void Menu::checkMouseClick(const sf::Vector2f &mousePos)
{
    if (m_startButton.getGlobalBounds().contains(mousePos))
    {
        m_selectedOption = 0;
        m_selectionMade = true;
        m_currentState = MenuState::Main;
    }
    else if (m_optionsButton.getGlobalBounds().contains(mousePos))
    {
        m_selectedOption = 1;
        m_selectionMade = true;
        m_currentState = MenuState::Options;
    }
    else if (m_exitButton.getGlobalBounds().contains(mousePos))
    {
        m_selectedOption = 2;
        m_selectionMade = true;
        m_currentState = MenuState::Exit;
    }
}
