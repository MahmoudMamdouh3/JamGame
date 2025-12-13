#include "GameObject.hpp"
#include <limits> 

GameObject::GameObject(const AssetData& data, std::string n)
    : sprite(data.texture), name(n)
{
    // Center origin for correct rotation
    sf::Vector2u size = data.texture.getSize();
    sprite.setOrigin({ size.x / 2.0f, size.y / 2.0f });
}

sf::FloatRect GameObject::getBounds() const {
    return sprite.getGlobalBounds();
}

std::vector<sf::Vector2f> GameObject::getWorldPoints() const {
    std::vector<sf::Vector2f> worldPoints;
    const sf::Transform& trans = sprite.getTransform();
    for (const auto& p : localPoints) {
        worldPoints.push_back(trans.transformPoint(p));
    }
    return worldPoints;
}

void GameObject::addPoint(sf::Vector2f worldPos) {
    sf::Transform inverse = sprite.getInverseTransform();
    localPoints.push_back(inverse.transformPoint(worldPos));
}

void GameObject::draw(sf::RenderWindow& window, bool isSelected, bool closeLoop) {
    // 1. Draw Sprite
    window.draw(sprite);

    // 2. Visual Feedback (Tint Colors)
    if (isTrap) sprite.setColor(sf::Color(255, 100, 100));       // Red = Trap
    else if (isWin) sprite.setColor(sf::Color(100, 255, 100));   // Green = Win
    else if (isSelected) sprite.setColor(sf::Color(200, 200, 255)); // Blue = Selected
    else sprite.setColor(sf::Color::White);

    // 3. Draw Red Collider Lines
    if (hasCollider || isSelected) {
        if (!localPoints.empty()) {
            std::vector<sf::Vector2f> points = getWorldPoints();
            sf::VertexArray lines(sf::PrimitiveType::LineStrip);

            for (const auto& p : points) {
                // SFML 3.0 Syntax
                lines.append(sf::Vertex{ p, sf::Color::Red });
            }

            if (closeLoop && points.size() > 2) {
                lines.append(sf::Vertex{ points[0], sf::Color::Red });
            }

            window.draw(lines);
        }
    }

    // 4. Draw Selection Box
    if (isSelected) {
        sf::RectangleShape box;
        auto b = getBounds();
        box.setPosition(b.position);
        box.setSize(b.size);
        box.setFillColor(sf::Color::Transparent);
        box.setOutlineColor(sf::Color::Yellow);
        box.setOutlineThickness(2.0f);
        window.draw(box);
    }
}