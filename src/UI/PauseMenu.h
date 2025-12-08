#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include "OptionsMenu.h"

enum class PauseMenuState
{
    Resume,
    Restart,
    Options,
    MainMenu
};

class PauseMenu
{
private:
    sf::RenderWindow &m_window;
    int m_selectedOption;
    bool m_optionsOpen;
    bool m_selectionMade;

    // Button properties
    sf::RectangleShape m_resumeButton;
    sf::RectangleShape m_restartButton;
    sf::RectangleShape m_optionsButton;
    sf::RectangleShape m_mainMenuButton;

    sf::Font m_font;
    std::unique_ptr<sf::Text> m_titleText;
    std::unique_ptr<sf::Text> m_resumeText;
    std::unique_ptr<sf::Text> m_restartText;
    std::unique_ptr<sf::Text> m_optionsText;
    std::unique_ptr<sf::Text> m_mainMenuText;

    // Semi-transparent overlay
    sf::RectangleShape m_overlay;

    // Colors
    const sf::Color m_buttonColor = sf::Color::White;
    const sf::Color m_selectedColor = sf::Color::Yellow;
    const sf::Color m_textColor = sf::Color::Black;

    // Options menu
    class OptionsMenu *m_optionsMenuPtr;

    sf::Vector2u m_lastWindowSize{};

public:
    PauseMenu(sf::RenderWindow &window);

    void handleInput();
    void render();

    int getSelectedOption() const { return m_selectedOption; }
    void resetSelection()
    {
        m_selectedOption = 0;
        m_optionsOpen = false;
        m_selectionMade = false;
    }
    void clearSelectionMade() { m_selectionMade = false; }
    bool isOptionsOpen() const { return m_optionsOpen; }
    bool isSelectionMade() const { return m_selectionMade; }
    void openOptions() { m_optionsOpen = true; }

private:
    void setupUI();
    void setupButtons();
    void setupText();
    void updateLayout();
    void updateTextPositions();
    void updateButtonSelection();
    bool isMouseOverButton(const sf::RectangleShape &button, const sf::Vector2f &mousePos) const;
    void checkMouseClick(const sf::Vector2f &mousePos);
};
