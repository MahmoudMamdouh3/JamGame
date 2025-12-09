#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include "OptionsMenu.h"

enum class MenuState
{
    Main,
    Options,
    Exit
};

class Menu
{
private:
    sf::RenderWindow &m_window;
    MenuState m_currentState;
    int m_selectedOption;
    bool m_selectionMade;

    // Main menu button positions and sizes
    sf::RectangleShape m_startButton;
    sf::RectangleShape m_optionsButton;
    sf::RectangleShape m_exitButton;

    sf::Font m_font;
    std::unique_ptr<sf::Text> m_startText;
    std::unique_ptr<sf::Text> m_optionsText;
    std::unique_ptr<sf::Text> m_exitText;
    std::unique_ptr<sf::Text> m_gameNameText;

    // Title textbox
    sf::RectangleShape m_titleBox;
    std::vector<std::unique_ptr<sf::Text>> m_titleLines;

    sf::Vector2u m_lastWindowSize{};

    // Options menu
    std::unique_ptr<OptionsMenu> m_optionsMenu;

    // Colors
    const sf::Color m_buttonColor = sf::Color::White;
    const sf::Color m_selectedColor = sf::Color::Yellow;
    const sf::Color m_textColor = sf::Color::Black;

public:
    Menu(sf::RenderWindow &window);

    void handleInput(AudioManager &audio);
    void render();

    MenuState getCurrentState() const { return m_currentState; }
    int getSelectedOption() const { return m_selectedOption; }
    bool isSelectionMade() const { return m_selectionMade; }
    float getVolume() const { return m_optionsMenu->getVolumeLevel(); }
    void resetSelection()
    {
        m_selectionMade = false;
        m_currentState = MenuState::Main;
    }

private:
    void setupButtons();
    void setupText();
    void setupTitleBox();
    void updateLayout();
    void updateTextPositions();
    void updateButtonSelection();
    bool isMouseOverButton(const sf::RectangleShape &button, const sf::Vector2f &mousePos) const;
    void checkMouseClick(const sf::Vector2f &mousePos);
    void wrapTextInBox(const std::string &text, float boxWidth, float charSize);
};
