#include "Map.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "../Tools/json.hpp" 

using json = nlohmann::json;

void Map::buildLevel(int levelIndex)
{
    m_props.clear();
    m_startPosition = { 0.f, 0.f };

    std::string filename = "level" + std::to_string(levelIndex) + ".json";
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "[MAP] Could not open " << filename << std::endl;
        return;
    }

    try {
        json root = json::parse(file);

        if (root.contains("playerStart")) {
            m_startPosition.x = root["playerStart"]["x"];
            m_startPosition.y = root["playerStart"]["y"];
        }

        json objects = root.contains("objects") ? root["objects"] : root;
        if (!objects.is_array()) return;

        for (const auto& item : objects) {
            std::string name = item["name"];
            std::string finalPath = "assets/" + name + ".png";

            // Fix for 04.png vs 4.png
            if (!std::filesystem::exists(finalPath)) {
                if (name.size() > 1 && name[0] == '0') {
                    std::string alt = "assets/" + name.substr(1) + ".png";
                    if (std::filesystem::exists(alt)) finalPath = alt;
                }
            }

            float x = (float)item["x"];
            float y = (float)item["y"];

            // Read Rotation & Scale
            float rotation = item.value("rot", 0.0f);
            float sx = item.value("scaleX", 1.0f);
            float sy = item.value("scaleY", sx);
            sf::Vector2f scale = { sx, sy };

            bool hasCollider = item.value("hasCollider", false);
            bool isWalkable = item.value("isWalkable", false);
            bool isTrap = item.value("isTrap", false);
            bool isWin = item.value("isWin", false);
            std::string audioFile = item.value("audioFile", "");

            std::vector<sf::Vector2f> points;
            if (item.contains("points")) {
                for (const auto& p : item["points"]) {
                    points.push_back(sf::Vector2f(p["x"], p["y"]));
                }
            }

            // Create Prop
            m_props.push_back(std::make_unique<Prop>(x, y, finalPath, scale, rotation, points,
                isWalkable, hasCollider,
                isTrap, isWin, audioFile));
        }
        std::cout << "[MAP] Loaded " << filename << " with " << m_props.size() << " props." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "JSON Error: " << e.what() << std::endl;
    }
}

void Map::update(float dt) {
    for (auto& prop : m_props) {
        prop->update(dt);
    }
}

void Map::render(sf::RenderWindow& window, const sf::Vector2f& cameraOffset) {
    for (const auto& prop : m_props) {
        prop->render(window, cameraOffset);
    }
}

bool Map::checkPropCollision(float x, float y) const {
    sf::Vector2f pos(x, y);
    for (const auto& prop : m_props) {
        if (prop->isColliding(pos)) return true;
    }
    return false;
}