#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>

const sf::Vector2u WINDOW_SIZE = { 1280, 720 };

struct AssetData {
    sf::Texture texture;
    std::vector<sf::Vector2f> collisionPoints; // CHANGED: Replaces defaultCollider
};

class ResourceManager {
private:
    std::map<std::string, AssetData> m_assets;
    sf::Font m_font;

public:
    ResourceManager();

    // Change the declaration to add a boolean flag
    void loadAsset(const std::string& name, const std::string& path, bool autoGenerate = false);
    void loadFont(const std::string& path);
    sf::Font& getFont();
    AssetData& getAsset(const std::string& name);
    const std::map<std::string, AssetData>& getAllAssets() const;

private:
    void ensureAssetExists(const std::string& filename, sf::Color color);
};

#endif