#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Tools/ResourceManager.hpp" // Check your path

struct GameObject {
    sf::Sprite sprite;
    std::string name;

    // --- LOGIC PROPERTIES ---
    bool hasCollider = false;  // 'C' Key: Does it have a shape?
    bool isWalkable = false;   // 'W' Key: Can player walk through it?

    // --- PUZZLE PROPERTIES ---
    bool isTrap = false;       // 'T' Key: Floor falls?
    bool isWin = false;        // 'E' Key: End Level?
    std::string audioFile = "";// 'U' Key: Sound to play

    // --- COLLIDER DATA ---
    std::vector<sf::Vector2f> localPoints;

    // Constructor
    GameObject(const AssetData& data, std::string n);

    // Core Functions
    sf::FloatRect getBounds() const;

    // Returns the collider points transformed to World Space
    std::vector<sf::Vector2f> getWorldPoints() const;

    // Helper to add points in Editor
    void addPoint(sf::Vector2f worldPos);

    // Draw Function (Handles Red Lines & Tints)
    void draw(sf::RenderWindow& window, bool isSelected, bool closeLoop);
};