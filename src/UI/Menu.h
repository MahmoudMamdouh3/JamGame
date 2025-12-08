#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include "AudioManager.h"
#include "OptionsMenu.h"
#include "Config.h" // GameState is defined here now

enum class MenuState
{
    Main,
    Options,
    Exit
};

class Menu
{
private:
    sf::RenderWindow& m_window;
    MenuState m_currentState;
    int m_selectedOption;
    bool m_selectionMade;

    sf::RectangleShape m_startButton;
    sf::RectangleShape m_optionsButton;
    sf::RectangleShape m_exitButton;

    sf::Font m_font;
    std::unique_ptr<sf::Text> m_startText;
    std::unique_ptr<sf::Text> m_optionsText;
    std::unique_ptr<sf::Text> m_exitText;
    std::unique_ptr<sf::Text> m_gameNameText;

    std::unique_ptr<OptionsMenu> m_optionsMenu;

    const sf::Color m_buttonColor = sf::Color::White;
    const sf::Color m_selectedColor = sf::Color::Yellow;
    const sf::Color m_textColor = sf::Color::Black;

public:
    Menu(sf::RenderWindow& window);

    void handleInput(AudioManager& audio);
    void render();

    MenuState getCurrentState() const { return m_currentState; }
    int getSelectedOption() const { return m_selectedOption; }
    bool isSelectionMade() const { return m_selectionMade; }

    void resetSelection()
    {
        m_selectionMade = false;
        m_currentState = MenuState::Main;
    }

private:
    void setupButtons();
    void setupText();
    void updateButtonSelection();
    bool isMouseOverButton(const sf::RectangleShape& button) const;
};