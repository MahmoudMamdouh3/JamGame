#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "../Props/Prop.h"

class Map {
public:
    Map() = default;

    // Load a specific level number
    void buildLevel(int levelIndex);

    // Update (for animations)
    void update(float dt);

    void render(sf::RenderWindow& window, const sf::Vector2f& cameraOffset);

    // Getters
    const std::vector<std::unique_ptr<Prop>>& getProps() const { return m_props; }

    // THIS FUNCTION NEEDS THE VARIABLE AT THE BOTTOM
    sf::Vector2f getStartPosition() const { return m_startPosition; }

    // Helpers
    float getHeight(int x, int y) const { return 0.0f; }
    bool checkPropCollision(float x, float y) const;

private:
    std::vector<std::unique_ptr<Prop>> m_props;

    // --- THIS VARIABLE MUST BE HERE ---
    sf::Vector2f m_startPosition = { 0.f, 0.f };
};