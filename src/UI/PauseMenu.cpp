#include "PauseMenu.h"
#include "Config.h"
#include <iostream>

PauseMenu::PauseMenu(sf::RenderWindow &window)
    : m_window(window), m_selectedOption(0), m_selectionMade(false), m_currentState(PauseMenuState::Main), m_optionsMenu(std::make_unique<OptionsMenu>(window))
{
    if (!m_font.openFromFile(FONT_PATH))
    {
        if (!m_font.openFromFile("C:\\Windows\\Fonts\\arial.ttf"))
        {
            std::cerr << "Failed to load font for Pause Menu" << std::endl;
        }
    }

    if (m_backgroundTexture.loadFromFile("assets/UI/UI Background.png"))
    {
        m_backgroundTexture.setSmooth(true);
        m_backgroundSprite.emplace(m_backgroundTexture);
    }

    // Initialize Title Text using unique_ptr
    m_titleText = std::make_unique<sf::Text>(m_font, "PAUSED", 60);
    // SFML 3 Fix: setPosition requires Vector2f
    m_titleText->setPosition({WINDOW_WIDTH / 2.f - 100.f, 200.f});

    std::vector<std::string> items = {"Resume", "Restart", "Options", "Menu"};
    for (size_t i = 0; i < items.size(); ++i)
    {
        auto text = std::make_unique<sf::Text>(m_font, items[i], 40);
        float optionY = 400.f + i * 60.f;
        text->setPosition({WINDOW_WIDTH / 2.f - 50.f, optionY});
        m_options.push_back(std::move(text));

        // Create clickable bounds for each option
        sf::RectangleShape bounds({250.f, 50.f});
        bounds.setPosition({WINDOW_WIDTH / 2.f - 125.f, optionY - 5.f});
        bounds.setFillColor(sf::Color::Transparent);
        m_optionBounds.push_back(bounds);
    }
}

void PauseMenu::resetSelection()
{
    m_selectedOption = 0;
    m_selectionMade = false;
    m_currentState = PauseMenuState::Main;
}

void PauseMenu::handleInput(AudioManager &audio)
{
    if (m_currentState == PauseMenuState::Options)
    {
        m_optionsMenu->handleInput(audio);

        if (m_optionsMenu->isBack())
        {
            m_currentState = PauseMenuState::Main;
            m_optionsMenu->resetSelection();
        }
    }
    else
    {
        static bool upPressed = false;
        static bool downPressed = false;
        static bool enterPressed = false;

        // SFML 3 Fix: sf::Keyboard::Key::Up
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            if (!upPressed)
            {
                m_selectedOption = (m_selectedOption - 1 + m_options.size()) % m_options.size();
                audio.playSound("menu_move");
                upPressed = true;
            }
        }
        else
        {
            upPressed = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            if (!downPressed)
            {
                m_selectedOption = (m_selectedOption + 1) % m_options.size();
                audio.playSound("menu_move");
                downPressed = true;
            }
        }
        else
        {
            downPressed = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
        {
            if (!enterPressed)
            {
                audio.playSound("menu_select");

                // Apply selection for keyboard the same way as mouse clicks
                if (m_selectedOption == 2)
                {
                    // Open options submenu
                    m_currentState = PauseMenuState::Options;
                    m_optionsMenu->resetSelection();
                    m_selectionMade = false;
                }
                else
                {
                    // Resume / Restart / Menu
                    m_selectionMade = true;
                }

                enterPressed = true;
            }
        }
        else
        {
            enterPressed = false;
        }

        // Handle mouse movement for option hover
        while (const std::optional event = m_window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                m_window.close();

            if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                const sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(mouseMoved->position.x, mouseMoved->position.y));

                for (size_t i = 0; i < m_optionBounds.size(); ++i)
                {
                    if (isMouseOverOption(m_optionBounds[i], mousePos))
                    {
                        if ((int)i != m_selectedOption)
                        {
                            m_selectedOption = i;
                            audio.playSound("menu_move");
                        }
                        break;
                    }
                }
            }

            // Handle mouse clicks
            if (const auto *mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButton->button == sf::Mouse::Button::Left)
                {
                    const sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(mouseButton->position.x, mouseButton->position.y));
                    checkMouseClick(mousePos, audio);
                }
            }
        }
    }
}

void PauseMenu::render()
{
    if (m_currentState == PauseMenuState::Options)
    {
        m_optionsMenu->render();
    }
    else
    {
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

        // Draw dark semi-transparent overlay
        sf::RectangleShape overlay({(float)WINDOW_WIDTH, (float)WINDOW_HEIGHT});
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        m_window.draw(overlay);

        if (m_titleText)
            m_window.draw(*m_titleText);

        for (size_t i = 0; i < m_options.size(); ++i)
        {
            if ((int)i == m_selectedOption)
                m_options[i]->setFillColor(sf::Color::Yellow);
            else
                m_options[i]->setFillColor(sf::Color::White);

            m_window.draw(*m_options[i]);
        }
    }
}

bool PauseMenu::isMouseOverOption(const sf::RectangleShape &bounds, const sf::Vector2f &mousePos) const
{
    const sf::Vector2f optionPos = bounds.getPosition();
    const sf::Vector2f optionSize = bounds.getSize();

    return mousePos.x >= optionPos.x && mousePos.x <= optionPos.x + optionSize.x &&
           mousePos.y >= optionPos.y && mousePos.y <= optionPos.y + optionSize.y;
}

void PauseMenu::checkMouseClick(const sf::Vector2f &mousePos, AudioManager &audio)
{
    for (size_t i = 0; i < m_optionBounds.size(); ++i)
    {
        if (isMouseOverOption(m_optionBounds[i], mousePos))
        {
            m_selectedOption = i;
            m_selectionMade = true;
            audio.playSound("menu_select");

            // If options was selected, transition to options menu
            if ((int)i == 2)
            {
                m_currentState = PauseMenuState::Options;
                m_selectionMade = false;
            }
            break;
        }
    }
}