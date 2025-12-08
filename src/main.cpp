#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

// Game states
enum class GameState
{
    MainMenu,
    Playing
};

// Convert isometric coordinates to screen coordinates
sf::Vector2f isometricToScreen(float isoX, float isoY, float tileWidth, float tileHeight)
{
    float screenX = (isoX - isoY) * (tileWidth / 2.f);
    float screenY = (isoX + isoY) * (tileHeight / 2.f);
    return {screenX, screenY};
}

// Convert screen coordinates to isometric coordinates
sf::Vector2f screenToIsometric(float screenX, float screenY, float tileWidth, float tileHeight)
{
    float isoX = (screenX / (tileWidth / 2.f) + screenY / (tileHeight / 2.f)) / 2.f;
    float isoY = (screenY / (tileHeight / 2.f) - screenX / (tileWidth / 2.f)) / 2.f;
    return {isoX, isoY};
}

// Button class for the menu
class Button
{
public:
    Button(sf::Vector2f position, sf::Vector2f size, const std::string &text, const sf::Font &font)
        : rect(size), label(font, text)
    {
        rect.setPosition(position);
        rect.setFillColor(sf::Color::Blue);
        rect.setOutlineThickness(2.f);
        rect.setOutlineColor(sf::Color::White);

        label.setCharacterSize(24);
        label.setFillColor(sf::Color::White);

        // Center text in button
        auto bounds = label.getLocalBounds();
        float textX = position.x + (size.x - bounds.size.x) / 2.f;
        float textY = position.y + (size.y - bounds.size.y) / 2.f;
        label.setPosition(sf::Vector2f(textX, textY));
    }

    bool contains(sf::Vector2f point) const
    {
        return rect.getGlobalBounds().contains(point);
    }

    void setHovered(bool hovered)
    {
        if (hovered)
            rect.setFillColor(sf::Color::Cyan);
        else
            rect.setFillColor(sf::Color::Blue);
    }

    void draw(sf::RenderWindow &window) const
    {
        window.draw(rect);
        window.draw(label);
    }

private:
    sf::RectangleShape rect;
    sf::Text label;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Isometric Game");
    window.setFramerateLimit(60);

    // Load a font from system paths
    sf::Font defaultFont;
    // Try common system font paths
    if (!defaultFont.openFromFile("C:/Windows/Fonts/arial.ttf") &&
        !defaultFont.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") &&
        !defaultFont.openFromFile("/System/Library/Fonts/Arial.ttf"))
    {
        // Font loading failed - text won't render but app will continue
        std::cerr << "Warning: Could not load any font file" << std::endl;
    }

    GameState gameState = GameState::MainMenu;

    // Isometric tile dimensions
    const float tileWidth = 64.f;
    const float tileHeight = 32.f;

    // Grid dimensions
    const int gridWidth = 30;
    const int gridHeight = 30;

    // Offset to center the grid on screen
    const sf::Vector2f gridOffset(300.f, 100.f);

    // Character position (in isometric grid coordinates)
    int charX = gridWidth / 2;
    int charY = gridHeight / 2;

    // Movement speed control
    int moveDelay = 0;
    const int moveSpeed = 8; // frames between moves

    // Mouse position tracking for highlight
    sf::Vector2i mousePos;
    int highlightX = -1, highlightY = -1;

    // Menu buttons
    Button playButton(sf::Vector2f(400.f, 300.f), sf::Vector2f(400.f, 80.f), "Play Game", defaultFont);
    Button exitButton(sf::Vector2f(400.f, 450.f), sf::Vector2f(400.f, 80.f), "Exit", defaultFont);

    // Main loop
    while (window.isOpen())
    {
        // Check all the window's events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (gameState == GameState::MainMenu)
            {
                if (const auto *mouseClick = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    sf::Vector2f mousePos(static_cast<float>(mouseClick->position.x),
                                          static_cast<float>(mouseClick->position.y));

                    if (playButton.contains(mousePos))
                    {
                        gameState = GameState::Playing;
                    }
                    if (exitButton.contains(mousePos))
                    {
                        window.close();
                    }
                }
            }
            else if (gameState == GameState::Playing)
            {
                if (event->is<sf::Event::MouseMoved>())
                {
                    mousePos = sf::Mouse::getPosition(window);

                    // Convert screen coordinates to isometric coordinates
                    sf::Vector2f screenPos(static_cast<float>(mousePos.x) - gridOffset.x,
                                           static_cast<float>(mousePos.y) - gridOffset.y);
                    sf::Vector2f isoPos = screenToIsometric(screenPos.x, screenPos.y, tileWidth, tileHeight);

                    highlightX = static_cast<int>(isoPos.x);
                    highlightY = static_cast<int>(isoPos.y);
                }

                // Return to menu with ESC key
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                {
                    gameState = GameState::MainMenu;
                }
            }
        }

        // Clear the window
        window.clear(sf::Color::Black);

        if (gameState == GameState::MainMenu)
        {
            // Draw menu title
            sf::Text title(defaultFont, "Isometric Game");
            title.setCharacterSize(48);
            title.setFillColor(sf::Color::White);
            title.setPosition(sf::Vector2f(1920 / 2, 1080 / 4));
            window.draw(title);

            // Update button hover state
            sf::Vector2f currentMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            playButton.setHovered(playButton.contains(currentMousePos));
            exitButton.setHovered(exitButton.contains(currentMousePos));

            // Draw buttons
            playButton.draw(window);
            exitButton.draw(window);
        }
        else if (gameState == GameState::Playing)
        {
            // Handle WASD input for character movement with speed control
            moveDelay++;
            if (moveDelay >= moveSpeed)
            {
                moveDelay = 0;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && charY > 0)
                    charY--;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && charY < gridHeight - 1)
                    charY++;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && charX > 0)
                    charX--;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && charX < gridWidth - 1)
                    charX++;
            }

            // Draw the isometric grid
            for (int y = 0; y < gridHeight; y++)
            {
                for (int x = 0; x < gridWidth; x++)
                {
                    // Convert isometric coordinates to screen coordinates
                    sf::Vector2f screenPos = isometricToScreen(static_cast<float>(x),
                                                               static_cast<float>(y),
                                                               tileWidth, tileHeight);
                    screenPos += gridOffset;

                    // Create a diamond shape for each tile
                    sf::ConvexShape tile(4);
                    tile.setPoint(0, sf::Vector2f(tileWidth / 2.f, 0));          // Top
                    tile.setPoint(1, sf::Vector2f(tileWidth, tileHeight / 2.f)); // Right
                    tile.setPoint(2, sf::Vector2f(tileWidth / 2.f, tileHeight)); // Bottom
                    tile.setPoint(3, sf::Vector2f(0, tileHeight / 2.f));         // Left

                    tile.setPosition(screenPos);

                    // Highlight the tile under the mouse
                    if (x == highlightX && y == highlightY &&
                        highlightX >= 0 && highlightY >= 0 &&
                        highlightX < gridWidth && highlightY < gridHeight)
                    {
                        tile.setFillColor(sf::Color::Yellow);
                        tile.setOutlineColor(sf::Color::White);
                    }
                    else
                    {
                        tile.setFillColor(sf::Color::Green);
                        tile.setOutlineColor(sf::Color::White);
                    }

                    tile.setOutlineThickness(1.f);

                    window.draw(tile);
                }
            }

            // Draw the character
            sf::Vector2f charScreenPos = isometricToScreen(static_cast<float>(charX),
                                                           static_cast<float>(charY),
                                                           tileWidth, tileHeight);
            charScreenPos += gridOffset;

            // Create a small circle for the character
            sf::CircleShape character(8.f);
            character.setPosition(sf::Vector2f(charScreenPos.x - 8.f, charScreenPos.y - 16.f));
            character.setFillColor(sf::Color::Red);
            window.draw(character);

            // Draw ESC hint
            sf::Text escHint(defaultFont, "Press ESC to return to menu");
            escHint.setCharacterSize(16);
            escHint.setFillColor(sf::Color::White);
            escHint.setPosition(sf::Vector2f(10.f, 10.f));
            window.draw(escHint);
        }

        window.display();
    }

    return 0;
}