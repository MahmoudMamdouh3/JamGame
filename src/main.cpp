#include <SFML/Graphics.hpp>
#include <glad/glad.h>
#include <box2d/box2d.h>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "JamGame");
    
    // Initialize Glad (must be done after window creation if using SFML's context)
    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // Simple Box2D Test
    b2WorldId worldId = b2CreateWorld(&b2_defaultWorldDef);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        // Draw stuff here
        window.display();
    }
    return 0;
}
