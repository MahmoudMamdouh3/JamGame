#include "PauseMenu.h"
#include "Config.h"
#include <iostream>

PauseMenu::PauseMenu(sf::RenderWindow& window)
    : m_window(window), m_selectedOption(0), m_selectionMade(false)
{
    if (!m_font.openFromFile(FONT_PATH)) {
        if (!m_font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            std::cerr << "Failed to load font for Pause Menu" << std::endl;
        }
    }

    // Initialize Title Text using unique_ptr
    m_titleText = std::make_unique<sf::Text>(m_font, "PAUSED", 60);
    // SFML 3 Fix: setPosition requires Vector2f
    m_titleText->setPosition({ WINDOW_WIDTH / 2.f - 100.f, 200.f });

    std::vector<std::string> items = { "Resume", "Restart", "Options", "Menu" };
    for (size_t i = 0; i < items.size(); ++i) {
        auto text = std::make_unique<sf::Text>(m_font, items[i], 40);
        text->setPosition({ WINDOW_WIDTH / 2.f - 50.f, 400.f + i * 60.f });
        m_options.push_back(std::move(text));
    }
}

void PauseMenu::resetSelection() {
    m_selectedOption = 0;
    m_selectionMade = false;
}

void PauseMenu::handleInput(AudioManager& audio) {
    static bool upPressed = false;
    static bool downPressed = false;
    static bool enterPressed = false;

    // SFML 3 Fix: sf::Keyboard::Key::Up
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        if (!upPressed) {
            m_selectedOption = (m_selectedOption - 1 + m_options.size()) % m_options.size();
            audio.playSound("menu_move");
            upPressed = true;
        }
    }
    else { upPressed = false; }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        if (!downPressed) {
            m_selectedOption = (m_selectedOption + 1) % m_options.size();
            audio.playSound("menu_move");
            downPressed = true;
        }
    }
    else { downPressed = false; }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
        if (!enterPressed) {
            m_selectionMade = true;
            audio.playSound("menu_select");
            enterPressed = true;
        }
    }
    else { enterPressed = false; }
}

void PauseMenu::render() {
    sf::RectangleShape overlay({ (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT });
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    m_window.draw(overlay);

    if (m_titleText) m_window.draw(*m_titleText);

    for (size_t i = 0; i < m_options.size(); ++i) {
        if ((int)i == m_selectedOption)
            m_options[i]->setFillColor(sf::Color::Yellow);
        else
            m_options[i]->setFillColor(sf::Color::White);

        m_window.draw(*m_options[i]);
    }
}