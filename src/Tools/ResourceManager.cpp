#include "ResourceManager.hpp"
#include "CollisionUtils.hpp" // INCLUDE THE NEW HELPER
#include <iostream>
#include <filesystem>

ResourceManager::ResourceManager() {
    ensureAssetExists("0.png", sf::Color::Green);
    ensureAssetExists("9.png", sf::Color(150, 50, 50));
    ensureAssetExists("wall.png", sf::Color(150, 50, 50));
}

void ResourceManager::loadAsset(const std::string& name, const std::string& path, bool autoGenerate) {
    if (!m_assets[name].texture.loadFromFile(path)) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return;
    }
    m_assets[name].texture.setSmooth(true);

    // ONLY generate if explicitly asked. Otherwise, it stays empty.
    if (autoGenerate) {
        sf::Image img = m_assets[name].texture.copyToImage();
        m_assets[name].collisionPoints = CollisionUtils::getAutoContour(img);
    }
    else {
        m_assets[name].collisionPoints.clear(); // Ensure it's empty
    }
}

AssetData& ResourceManager::getAsset(const std::string& name) {
    return m_assets[name];
}

const std::map<std::string, AssetData>& ResourceManager::getAllAssets() const {
    return m_assets;
}

sf::Font& ResourceManager::getFont() {
    // Return immediately if already loaded
    static sf::Font font;
    static bool loaded = false;

    if (loaded) return font;

    // Define the path we WANT to find
    std::string path = "assets/arial.ttf"; // <--- CHANGE THIS to your actual font name (e.g., font.ttf)

    // Try to load
    if (!font.openFromFile(path)) {
        // --- DETECTIVE MODE LOGGING ---
        std::cerr << "========================================" << std::endl;
        std::cerr << "[ERROR] Failed to load FONT!" << std::endl;
        std::cerr << "    Provided path: " << path << std::endl;

        // This prints the FULL path on your hard drive
        try {
            std::cerr << "    Absolute path: " << std::filesystem::absolute(path).string() << std::endl;
        }
        catch (...) {
            std::cerr << "    (Could not determine absolute path)" << std::endl;
        }

        std::cerr << "----------------------------------------" << std::endl;
        std::cerr << "FIX: Copy '" << path << "' to the folder above!" << std::endl;
        std::cerr << "========================================" << std::endl;

        // Fallback: Load a default font or crash gracefully
        // exit(-1); // Optional: Stop game immediately
    }
    else {
        std::cout << "[SUCCESS] Font loaded from: " << path << std::endl;
        loaded = true;
    }

    return font;
}



void ResourceManager::ensureAssetExists(const std::string& filename, sf::Color color) {
    if (!std::filesystem::exists(filename)) {
        sf::Image img;
        img.resize({ 32, 32 }, color);
        img.saveToFile(filename);
    }
}