#include "Menu.h"
#include "../Core/Config.h"
#include "OptionsMenu.h"
#include <iostream>
#include <sstream>
#include "../Audio/AudioManager.h"

Menu::Menu(sf::RenderWindow &window)
    : m_window(window), m_currentState(MenuState::Main), m_selectedOption(0), m_selectionMade(false), m_optionsMenu(std::make_unique<OptionsMenu>(window))
{
    setupButtons();
    setupText();
    setupTitleBox();
    updateLayout();
}

void Menu::setupButtons()
{
    // Button dimensions
    float buttonWidth = 300.f;
    float buttonHeight = 80.f;
    float leftMargin = 100.f;
    float startY = static_cast<float>(m_window.getSize().y) / 2.f - 150.f;
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
    if (!m_font.openFromFile(FONT_PATH))
    {
        if (!m_font.openFromFile("C:\\Windows\\Fonts\\arial.ttf"))
            std::cerr << "Failed to load font" << std::endl;
    }

    // Start Text
    m_startText = std::make_unique<sf::Text>(m_font, "START", 40);
    m_startText->setFillColor(m_textColor);

    // Options Text
    m_optionsText = std::make_unique<sf::Text>(m_font, "OPTIONS", 40);
    m_optionsText->setFillColor(m_textColor);

    // Exit Text
    m_exitText = std::make_unique<sf::Text>(m_font, "EXIT", 40);
    m_exitText->setFillColor(m_textColor);

    updateTextPositions();
}

void Menu::setupTitleBox()
{
    // Create title box on the right side
    float boxWidth = 400.f;
    float boxHeight = 300.f;
    float boxX = static_cast<float>(m_window.getSize().x) - boxWidth - 50.f; // 50 pixels from right edge
    float boxY = static_cast<float>(m_window.getSize().y) / 2.f - boxHeight / 2;

    m_titleBox.setSize({boxWidth, boxHeight});
    m_titleBox.setPosition({boxX, boxY});
    m_titleBox.setFillColor(sf::Color::Transparent);
    m_titleBox.setOutlineColor(sf::Color::White);
    m_titleBox.setOutlineThickness(2.f);

    // Wrap text inside the box
    wrapTextInBox(GAME_TITLE, boxWidth - 20.f, 50);
}

void Menu::wrapTextInBox(const std::string &text, float boxWidth, float charSize)
{
    m_titleLines.clear();

    std::string currentLine;
    std::istringstream stream(text);
    std::string word;

    while (stream >> word)
    {
        std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
        sf::Text testText(m_font, testLine, static_cast<unsigned int>(charSize));

        if (testText.getGlobalBounds().size.x > boxWidth && !currentLine.empty())
        {
            // Current line is full, start a new one
            auto lineText = std::make_unique<sf::Text>(m_font, currentLine, static_cast<unsigned int>(charSize));
            lineText->setFillColor(sf::Color::White);
            m_titleLines.push_back(std::move(lineText));
            currentLine = word;
        }
        else
        {
            currentLine = testLine;
        }
    }

    // Add the last line
    if (!currentLine.empty())
    {
        auto lineText = std::make_unique<sf::Text>(m_font, currentLine, static_cast<unsigned int>(charSize));
        lineText->setFillColor(sf::Color::White);
        m_titleLines.push_back(std::move(lineText));
    }

    // Position each line inside the box
    float lineY = m_titleBox.getPosition().y + 15.f;
    float lineSpacing = charSize + 10.f;

    for (auto &line : m_titleLines)
    {
        line->setPosition({m_titleBox.getPosition().x + 10.f, lineY});
        lineY += lineSpacing;
    }
}

void Menu::updateTextPositions()
{
    if (m_startText)
        m_startText->setPosition(m_startButton.getPosition() + sf::Vector2f{60.f, 15.f});
    if (m_optionsText)
        m_optionsText->setPosition(m_optionsButton.getPosition() + sf::Vector2f{35.f, 15.f});
    if (m_exitText)
        m_exitText->setPosition(m_exitButton.getPosition() + sf::Vector2f{80.f, 15.f});
}

void Menu::updateLayout()
{
    const sf::Vector2u currentSize = m_window.getSize();
    if (currentSize == m_lastWindowSize || currentSize.x == 0 || currentSize.y == 0)
        return;

    m_lastWindowSize = currentSize;

    float buttonWidth = 300.f;
    float buttonHeight = 80.f;
    float leftMargin = 100.f;
    float startY = static_cast<float>(currentSize.y) / 2.f - 150.f;
    float spacing = 120.f;

    m_startButton.setSize({buttonWidth, buttonHeight});
    m_startButton.setPosition({leftMargin, startY});

    m_optionsButton.setSize({buttonWidth, buttonHeight});
    m_optionsButton.setPosition({leftMargin, startY + spacing});

    m_exitButton.setSize({buttonWidth, buttonHeight});
    m_exitButton.setPosition({leftMargin, startY + spacing * 2});

    updateTextPositions();

    float boxWidth = 400.f;
    float boxHeight = 300.f;
    float boxX = static_cast<float>(currentSize.x) - boxWidth - 50.f;
    float boxY = static_cast<float>(currentSize.y) / 2.f - boxHeight / 2.f;

    m_titleBox.setSize({boxWidth, boxHeight});
    m_titleBox.setPosition({boxX, boxY});

    wrapTextInBox(GAME_TITLE, boxWidth - 20.f, 50);
}

void Menu::handleInput(AudioManager &audio)
{
    updateLayout();

    if (m_currentState == MenuState::Options)
    {
        m_optionsMenu->handleInput(audio);

        if (m_optionsMenu->isBack())
        {
            m_currentState = MenuState::Main;
            m_optionsMenu->resetSelection();
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
                // Navigate with arrow keys
                if (key->code == sf::Keyboard::Key::Up || key->code == sf::Keyboard::Key::W)
                {
                    m_selectedOption = (m_selectedOption - 1 + 3) % 3;
                    audio.playSound("menu_move");
                }
                else if (key->code == sf::Keyboard::Key::Down || key->code == sf::Keyboard::Key::S)
                {
                    m_selectedOption = (m_selectedOption + 1) % 3;
                    audio.playSound("menu_move");
                }
                // Select with Enter or Space
                else if (key->code == sf::Keyboard::Key::Enter || key->code == sf::Keyboard::Key::Space)
                {
                    audio.playSound("menu_select");
                    if (m_selectedOption == 0)
                    {
                        m_selectionMade = true;
                        m_currentState = MenuState::Main; // Start game
                    }
                    else if (m_selectedOption == 1)
                        m_currentState = MenuState::Options; // Options
                    else if (m_selectedOption == 2)
                    {
                        m_selectionMade = true;
                        m_currentState = MenuState::Exit; // Exit
                    }
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
                const sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(mouseMoved->position.x, mouseMoved->position.y));

                if (isMouseOverButton(m_startButton, mousePos))
                {
                    m_selectedOption = 0;
                    audio.playSound("menu_move");
                }
                else if (isMouseOverButton(m_optionsButton, mousePos))
                {
                    m_selectedOption = 1;
                    audio.playSound("menu_move");
                }
                else if (isMouseOverButton(m_exitButton, mousePos))
                {
                    m_selectedOption = 2;
                    audio.playSound("menu_move");
                }
            }

            // Handle mouse clicks
            if (const auto *mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButton->button == sf::Mouse::Button::Left)
                {
                    const sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(mouseButton->position.x, mouseButton->position.y));
                    checkMouseClick(mousePos);
                    audio.playSound("menu_select");
                }
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
    updateLayout();

    if (m_currentState == MenuState::Options)
    {
        m_optionsMenu->render();
    }
    else
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

        // Render title box
        m_window.draw(m_titleBox);

        // Render wrapped title text
        for (auto &line : m_titleLines)
        {
            if (line)
                m_window.draw(*line);
        }
    }
    m_window.display();
}

bool Menu::isMouseOverButton(const sf::RectangleShape &button, const sf::Vector2f &mousePos) const
{
    const sf::Vector2f buttonPos = button.getPosition();
    const sf::Vector2f buttonSize = button.getSize();

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
        m_currentState = MenuState::Options;
    }
    else if (m_exitButton.getGlobalBounds().contains(mousePos))
    {
        m_selectedOption = 2;
        m_selectionMade = true;
        m_currentState = MenuState::Exit;
    }
}
