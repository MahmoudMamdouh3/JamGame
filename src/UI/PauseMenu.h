#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "AudioManager.h"

class PauseMenu {
private:
    sf::RenderWindow& m_window;
    sf::Font m_font;

    // SFML 3 Fix: Use pointers for Text to avoid "no default constructor" error
    std::unique_ptr<sf::Text> m_titleText;
    std::vector<std::unique_ptr<sf::Text>> m_options;

    int m_selectedOption;
    bool m_selectionMade;

public:
    PauseMenu(sf::RenderWindow& window);

    void handleInput(AudioManager& audio);
    void render();
    void resetSelection();

    bool isSelectionMade() const { return m_selectionMade; }
    int getSelectedOption() const { return m_selectedOption; }
    void clearSelectionMade() { m_selectionMade = false; }
};