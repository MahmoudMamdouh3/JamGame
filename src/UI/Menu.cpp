#include "Menu.h"
#include <algorithm>
#include <iostream>
#include "OptionsMenu.h"
#include "../Audio/AudioManager.h"

Menu::Menu(sf::RenderWindow &window)
    : m_window(window), m_currentState(MenuState::Main), m_selectedOption(0), m_selectionMade(false), m_optionsMenu(std::make_unique<OptionsMenu>(window))
{
    setupAssets();
    updateLayout();
}

void Menu::setupAssets()
{
    auto loadTexture = [](sf::Texture &texture, const std::string &path)
    {
        if (!texture.loadFromFile(path))
            std::cerr << "Failed to load texture: " << path << std::endl;
        else
            texture.setSmooth(true);
    };

    loadTexture(m_backgroundTexture, "assets/UI/UI Background.png");
    if (m_backgroundTexture.getSize().x > 0 && m_backgroundTexture.getSize().y > 0)
        m_backgroundSprite.emplace(m_backgroundTexture);

    const auto makeButton = [&](sf::Texture &texture, const std::string &path) -> std::optional<sf::Sprite>
    {
        loadTexture(texture, path);
        const sf::Vector2u texSize = texture.getSize();
        if (texSize.x == 0 || texSize.y == 0)
            return std::nullopt;

        sf::Sprite sprite(texture);
        sprite.setColor(m_buttonColor);

        const float desiredWidth = 620.f; // upscale texture target size
        const float desiredHeight = 180.f;
        const float scale = std::min(desiredWidth / static_cast<float>(texSize.x),
                                     desiredHeight / static_cast<float>(texSize.y));
        sprite.setScale(sf::Vector2f{scale, scale});
        return sprite;
    };

    m_startSprite = makeButton(m_startTexture, "assets/UI/Start Button.png");
    if (m_startSprite)
        m_startBaseScale = m_startSprite->getScale();

    m_optionsSprite = makeButton(m_optionsTexture, "assets/UI/Options Button.png");
    if (m_optionsSprite)
        m_optionsBaseScale = m_optionsSprite->getScale();

    m_exitSprite = makeButton(m_exitTexture, "assets/UI/Exit Button.png");
    if (m_exitSprite)
        m_exitBaseScale = m_exitSprite->getScale();
}

void Menu::updateLayout()
{
    const sf::Vector2u currentSize = m_window.getSize();
    if (currentSize == m_lastWindowSize || currentSize.x == 0 || currentSize.y == 0)
        return;

    m_lastWindowSize = currentSize;

    float leftMargin = 240.f; // shift buttons further right
    float startY = static_cast<float>(currentSize.y) / 2.f - 230.f;
    float spacing = 210.f;

    if (m_backgroundSprite && m_backgroundTexture.getSize().x > 0 && m_backgroundTexture.getSize().y > 0)
    {
        m_backgroundSprite->setScale(sf::Vector2f{
            static_cast<float>(currentSize.x) / m_backgroundTexture.getSize().x,
            static_cast<float>(currentSize.y) / m_backgroundTexture.getSize().y});
    }

    if (m_startSprite)
        m_startSprite->setPosition({leftMargin, startY});
    if (m_optionsSprite)
        m_optionsSprite->setPosition({leftMargin, startY + spacing});
    if (m_exitSprite)
        m_exitSprite->setPosition({leftMargin, startY + spacing * 2});
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

                if (isMouseOverButton(m_startSprite, mousePos))
                {
                    m_selectedOption = 0;
                    audio.playSound("menu_move");
                }
                else if (isMouseOverButton(m_optionsSprite, mousePos))
                {
                    m_selectedOption = 1;
                    audio.playSound("menu_move");
                }
                else if (isMouseOverButton(m_exitSprite, mousePos))
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
    const auto applyState = [&](std::optional<sf::Sprite> &sprite, const std::optional<sf::Vector2f> &baseScale, bool selected)
    {
        if (!sprite || !baseScale)
            return;

        const sf::Vector2f targetScale = selected ? sf::Vector2f{baseScale->x * m_highlightScale, baseScale->y * m_highlightScale}
                                                  : *baseScale;
        sprite->setScale(targetScale);
        sprite->setColor(selected ? sf::Color(255, 255, 255, 255) : m_buttonColor);
    };

    applyState(m_startSprite, m_startBaseScale, m_selectedOption == 0);
    applyState(m_optionsSprite, m_optionsBaseScale, m_selectedOption == 1);
    applyState(m_exitSprite, m_exitBaseScale, m_selectedOption == 2);
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

        // Background
        if (m_backgroundSprite)
            m_window.draw(*m_backgroundSprite);

        // Render buttons
        if (m_startSprite)
            m_window.draw(*m_startSprite);
        if (m_optionsSprite)
            m_window.draw(*m_optionsSprite);
        if (m_exitSprite)
            m_window.draw(*m_exitSprite);
    }
    m_window.display();
}

bool Menu::isMouseOverButton(const std::optional<sf::Sprite> &button, const sf::Vector2f &mousePos) const
{
    return button && button->getGlobalBounds().contains(mousePos);
}

void Menu::checkMouseClick(const sf::Vector2f &mousePos)
{
    if (m_startSprite && m_startSprite->getGlobalBounds().contains(mousePos))
    {
        m_selectedOption = 0;
        m_selectionMade = true;
        m_currentState = MenuState::Main;
    }
    else if (m_optionsSprite && m_optionsSprite->getGlobalBounds().contains(mousePos))
    {
        m_selectedOption = 1;
        m_currentState = MenuState::Options;
    }
    else if (m_exitSprite && m_exitSprite->getGlobalBounds().contains(mousePos))
    {
        m_selectedOption = 2;
        m_selectionMade = true;
        m_currentState = MenuState::Exit;
    }
}
