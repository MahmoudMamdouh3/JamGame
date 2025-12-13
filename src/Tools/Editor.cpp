#include <memory> // Add this at the very top of Editor.cpp!
#include "Editor.hpp"
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "CollisionUtils.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

using json = nlohmann::json;

sf::Vector2f g_playerStartPos = { 0.f, 0.f };

Editor::Editor(sf::RenderWindow& window, ResourceManager& resources)
    : m_window(window), m_resources(resources)
{
    setupUI();
    setupPalette();
    setupAudioList();
    loadCurrentLevel();
}

void Editor::setupAudioList() {
    m_audioOptions = { "", "move_up.wav", "move_right.wav", "fall.wav", "win.wav" };
}


void Editor::cycleAudio() {
    if (!m_selectedObject) return;

    // Cycle through names
    auto it = std::find(m_audioOptions.begin(), m_audioOptions.end(), m_selectedObject->audioFile);
    size_t index = (it != m_audioOptions.end()) ? std::distance(m_audioOptions.begin(), it) : 0;
    index = (index + 1) % m_audioOptions.size();
    m_selectedObject->audioFile = m_audioOptions[index];

    // Play Preview (Safe for SFML 3.0)
    if (!m_selectedObject->audioFile.empty()) {
        static sf::SoundBuffer buffer;
        std::string path = "assets/" + m_selectedObject->audioFile;

        if (buffer.loadFromFile(path)) {
            // Use unique_ptr so we don't create an empty sound object globally
            static std::unique_ptr<sf::Sound> sound;

            if (!sound) sound = std::make_unique<sf::Sound>(buffer);
            else sound->setBuffer(buffer);

            sound->play();
            std::cout << "[EDITOR] Previewing: " << path << std::endl;
        }
        else {
            std::cout << "[EDITOR] Error: Could not load " << path << std::endl;
        }
    }
}

void Editor::changeLevel(int newLevel) {
    if (newLevel < 1 || newLevel > MAX_LEVELS) return;
    if (newLevel == m_currentLevelIndex) return;
    saveCurrentLevel();
    m_currentLevelIndex = newLevel;
    m_selectedObject = nullptr;
    m_isEditingCollider = false;
    loadCurrentLevel();
}

void Editor::saveCurrentLevel() {
    std::string filename = "level" + std::to_string(m_currentLevelIndex) + ".json";

    json root = json::object();
    root["playerStart"]["x"] = g_playerStartPos.x;
    root["playerStart"]["y"] = g_playerStartPos.y;

    json objects = json::array();

    for (const auto& obj : m_sceneObjects) {
        json item;
        item["name"] = obj.name;
        item["x"] = obj.sprite.getPosition().x;
        item["y"] = obj.sprite.getPosition().y;
        item["rot"] = obj.sprite.getRotation().asDegrees();
        item["scaleX"] = obj.sprite.getScale().x;
        item["scaleY"] = obj.sprite.getScale().y;

        item["hasCollider"] = obj.hasCollider;
        item["isWalkable"] = obj.isWalkable;
        item["isTrap"] = obj.isTrap;
        item["isWin"] = obj.isWin;
        item["audioFile"] = obj.audioFile;

        if (!obj.localPoints.empty()) {
            item["points"] = json::array();
            for (const auto& p : obj.localPoints) {
                item["points"].push_back({ {"x", p.x}, {"y", p.y} });
            }
        }
        objects.push_back(item);
    }

    root["objects"] = objects;

    std::ofstream file(filename);
    if (file.is_open()) file << root.dump(4);
    std::cout << "[EDITOR] Saved " << filename << std::endl;
}

void Editor::loadCurrentLevel() {
    std::string filename = "level" + std::to_string(m_currentLevelIndex) + ".json";
    std::ifstream file(filename);

    m_sceneObjects.clear();
    m_selectedObject = nullptr;
    g_playerStartPos = { 0.f, 0.f };

    if (!file.is_open()) return;

    try {
        json root = json::parse(file);

        if (root.contains("playerStart")) {
            g_playerStartPos.x = root["playerStart"]["x"];
            g_playerStartPos.y = root["playerStart"]["y"];
        }

        json objects = root.contains("objects") ? root["objects"] : root;

        if (objects.is_array()) {
            for (const auto& item : objects) {
                std::string name = item["name"];
                try {
                    GameObject newObj(m_resources.getAsset(name), name);
                    newObj.sprite.setPosition({ (float)item["x"], (float)item["y"] });
                    newObj.sprite.setRotation(sf::degrees((float)item.value("rot", 0.0f)));

                    float sx = item.value("scaleX", 1.0f);
                    float sy = item.value("scaleY", sx);
                    newObj.sprite.setScale({ sx, sy });

                    newObj.hasCollider = item.value("hasCollider", false);
                    newObj.isWalkable = item.value("isWalkable", false);
                    newObj.isTrap = item.value("isTrap", false);
                    newObj.isWin = item.value("isWin", false);
                    newObj.audioFile = item.value("audioFile", "");

                    if (item.contains("points")) {
                        for (const auto& p : item["points"]) {
                            newObj.localPoints.push_back({ (float)p["x"], (float)p["y"] });
                        }
                    }
                    m_sceneObjects.push_back(newObj);
                }
                catch (...) {}
            }
        }
    }
    catch (...) {}
}

void Editor::selectObjectAt(sf::Vector2f pos) {
    m_selectedObject = nullptr;
    // Reverse loop to select top-most object first
    for (auto it = m_sceneObjects.rbegin(); it != m_sceneObjects.rend(); ++it) {
        if (it->getBounds().contains(pos)) {
            m_selectedObject = &(*it);

            // --- ENABLE DRAGGING ---
            m_isDragging = true;
            m_dragOffset = m_selectedObject->sprite.getPosition() - pos;
            break;
        }
    }
}

void Editor::handleMouseClick(sf::Mouse::Button button, sf::Vector2i pixelPos) {
    sf::Vector2f worldPos = m_window.mapPixelToCoords(pixelPos);

    if (pixelPos.y < 50 && pixelPos.x > SIDEBAR_WIDTH && pixelPos.x < (1920 - INFO_PANEL_WIDTH)) {
        int clickedLevel = ((pixelPos.x - SIDEBAR_WIDTH) / 60) + 1;
        if (clickedLevel >= 1 && clickedLevel <= MAX_LEVELS) changeLevel(clickedLevel);
        return;
    }

    if (button == sf::Mouse::Button::Right) {
        m_selectedObject = nullptr;
        m_isEditingCollider = false;
        m_currentBrushName = ""; // <--- FIX: Stop placing assets
        m_isDragging = false;    // <--- FIX: Stop dragging immediately
        return;
    }

    if (m_isEditingCollider && m_selectedObject && button == sf::Mouse::Button::Left) {
        if (pixelPos.x > SIDEBAR_WIDTH && pixelPos.x < (1920 - INFO_PANEL_WIDTH)) {
            m_selectedObject->addPoint(worldPos);
            return;
        }
    }

    if (pixelPos.x < SIDEBAR_WIDTH) {
        float scrollY = worldPos.y + m_sidebarScroll;
        for (const auto& icon : m_paletteIcons) {
            if (icon.first.getGlobalBounds().contains({ worldPos.x, scrollY })) {
                m_currentBrushName = icon.second;
                m_selectedObject = nullptr;
                m_isEditingCollider = false;
            }
        }
    }
    else if (pixelPos.x < (1920 - INFO_PANEL_WIDTH)) {
        if (!m_currentBrushName.empty()) {
            GameObject newObj(m_resources.getAsset(m_currentBrushName), m_currentBrushName);
            newObj.sprite.setPosition(worldPos);
            m_sceneObjects.push_back(newObj);
            m_selectedObject = &m_sceneObjects.back();
            // Auto-select new object to drag immediately
            m_isDragging = true;
            m_dragOffset = { 0.f, 0.f };
        }
        else {
            selectObjectAt(worldPos);
        }
    }
}

void Editor::handleKeyboard(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Key::F5) saveCurrentLevel();

    if (key == sf::Keyboard::Key::P) {
        sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
        g_playerStartPos = mousePos;
        std::cout << "Player Start Set: " << g_playerStartPos.x << "," << g_playerStartPos.y << std::endl;
    }

    // Must have an object selected to do anything else!
    if (!m_selectedObject) return;

    if (key == sf::Keyboard::Key::Tab) {
        m_isEditingCollider = !m_isEditingCollider;
        if (m_isEditingCollider) m_selectedObject->hasCollider = true;
    }

    if (m_isEditingCollider) {
        if (key == sf::Keyboard::Key::A) {
            sf::Image img = m_selectedObject->sprite.getTexture().copyToImage();
            m_selectedObject->localPoints = CollisionUtils::getAutoContour(img);
        }
        if (key == sf::Keyboard::Key::Z && !m_selectedObject->localPoints.empty()) m_selectedObject->localPoints.pop_back();
        return;
    }

    if (key == sf::Keyboard::Key::Delete) {
        auto it = std::remove_if(m_sceneObjects.begin(), m_sceneObjects.end(),
            [this](const GameObject& obj) { return &obj == m_selectedObject; });
        m_sceneObjects.erase(it, m_sceneObjects.end());
        m_selectedObject = nullptr;
        return;
    }

    // --- MOVEMENT (Arrows) ---
    float speed = 5.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) speed = 1.0f;

    if (key == sf::Keyboard::Key::Left) m_selectedObject->sprite.move({ -speed, 0.f });
    if (key == sf::Keyboard::Key::Right) m_selectedObject->sprite.move({ speed, 0.f });
    if (key == sf::Keyboard::Key::Up) m_selectedObject->sprite.move({ 0.f, -speed });
    if (key == sf::Keyboard::Key::Down) m_selectedObject->sprite.move({ 0.f, speed });

    // --- ROTATION (PageUp/Down OR < >) ---
    if (key == sf::Keyboard::Key::PageUp || key == sf::Keyboard::Key::Period)
        m_selectedObject->sprite.rotate(sf::degrees(5.f));

    if (key == sf::Keyboard::Key::PageDown || key == sf::Keyboard::Key::Comma)
        m_selectedObject->sprite.rotate(sf::degrees(-5.f));

    // --- SCALING (Home/End OR +/-) ---
    // Increase Size
    if (key == sf::Keyboard::Key::Home ||
        key == sf::Keyboard::Key::Add ||
        key == sf::Keyboard::Key::Equal) // The '+' key next to Backspace 
    {
        m_selectedObject->sprite.scale({ 1.1f, 1.1f });
    }

    // Decrease Size
    if (key == sf::Keyboard::Key::End ||
        key == sf::Keyboard::Key::Subtract ||
        key == sf::Keyboard::Key::Hyphen) // The '-' key next to 0
    {
        m_selectedObject->sprite.scale({ 0.9f, 0.9f });
    }

    // Properties
    if (key == sf::Keyboard::Key::C) m_selectedObject->hasCollider = !m_selectedObject->hasCollider;
    if (key == sf::Keyboard::Key::W) m_selectedObject->isWalkable = !m_selectedObject->isWalkable;
    if (key == sf::Keyboard::Key::T) m_selectedObject->isTrap = !m_selectedObject->isTrap;
    if (key == sf::Keyboard::Key::E) m_selectedObject->isWin = !m_selectedObject->isWin;
    if (key == sf::Keyboard::Key::U) cycleAudio();
}

void Editor::update() {
    // --- DRAGGING LOGIC ---
    if (m_isDragging && m_selectedObject && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));

        // Ensure we don't drag into the sidebar
        if (mousePos.x > SIDEBAR_WIDTH && mousePos.x < (1920 - INFO_PANEL_WIDTH)) {
            m_selectedObject->sprite.setPosition(mousePos + m_dragOffset);
        }
    }
    else {
        m_isDragging = false;
    }
}

void Editor::handleEvent(const sf::Event& event) {
    if (const auto* wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (sf::Mouse::getPosition(m_window).x < SIDEBAR_WIDTH) {
            m_sidebarScroll = std::max(0.f, m_sidebarScroll - wheel->delta * 40.f);
        }
    }
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) handleMouseClick(mouse->button, mouse->position);

    // Safety release
    if (const auto* up = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (up->button == sf::Mouse::Button::Left) m_isDragging = false;
    }

    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) handleKeyboard(key->code);
}

void Editor::setupUI() {
    sf::Vector2f winSize{ 1920.f, 1080.f };
    m_sidebarLeft.setSize({ SIDEBAR_WIDTH, winSize.y });
    m_sidebarLeft.setFillColor(sf::Color(30, 30, 30));
    m_sidebarRight.setSize({ INFO_PANEL_WIDTH, winSize.y });
    m_sidebarRight.setPosition({ winSize.x - INFO_PANEL_WIDTH, 0.f });
    m_sidebarRight.setFillColor(sf::Color(30, 30, 30));
    m_topBar.setSize({ winSize.x - SIDEBAR_WIDTH - INFO_PANEL_WIDTH, 50.f });
    m_topBar.setPosition({ SIDEBAR_WIDTH, 0.f });
    m_topBar.setFillColor(sf::Color(50, 50, 60));
}

void Editor::drawLevelSelector() {
    m_window.draw(m_topBar);
    for (int i = 1; i <= MAX_LEVELS; ++i) {
        sf::RectangleShape btn({ 50.f, 40.f });
        btn.setPosition({ SIDEBAR_WIDTH + (i - 1) * 60.f + 5.f, 5.f });
        btn.setFillColor(i == m_currentLevelIndex ? sf::Color::Green : sf::Color(100, 100, 100));
        m_window.draw(btn);
        sf::Text num(m_resources.getFont(), std::to_string(i), 20);
        num.setPosition({ btn.getPosition().x + 15.f, btn.getPosition().y + 5.f });
        m_window.draw(num);
    }
}

void Editor::drawProperties() {
    m_window.draw(m_sidebarRight);

    float startX = 1920.f - INFO_PANEL_WIDTH + 20.f;
    float y = 20.f;

    sf::Text t(m_resources.getFont(), "PROPERTIES", 24);
    t.setPosition({ startX, y });
    m_window.draw(t);
    y += 40.f;

    if (m_selectedObject) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1);

        auto pos = m_selectedObject->sprite.getPosition();
        auto scale = m_selectedObject->sprite.getScale();

        ss << "Name: " << m_selectedObject->name << "\n\n";
        ss << "Pos:   " << pos.x << ", " << pos.y << "\n";
        ss << "Rot:   " << m_selectedObject->sprite.getRotation().asDegrees() << "\n";
        ss << "Scale: " << scale.x << ", " << scale.y << "\n\n";
        ss << "[C] Collider: " << (m_selectedObject->hasCollider ? "ON" : "OFF") << "\n";
        ss << "[W] Walkable: " << (m_selectedObject->isWalkable ? "YES" : "NO") << "\n";
        ss << "[T] Trap:     " << (m_selectedObject->isTrap ? "YES" : "NO") << "\n";
        ss << "[E] Win:      " << (m_selectedObject->isWin ? "YES" : "NO") << "\n";
        ss << "[U] Audio:    " << (m_selectedObject->audioFile.empty() ? "None" : m_selectedObject->audioFile) << "\n";

        if (m_isEditingCollider) {
            ss << "\n[EDITING COLLIDER MODE]\n[A] Auto-Gen\n[Tab] Exit";
        }

        sf::Text props(m_resources.getFont(), ss.str(), 18);
        props.setPosition({ startX, y });
        m_window.draw(props);
    }
}

void Editor::setupPalette() {
    float y = 60.f;
    for (auto const& [name, asset] : m_resources.getAllAssets()) {
        sf::Sprite s(asset.texture);
        float scale = 64.f / std::max(asset.texture.getSize().x, asset.texture.getSize().y);
        s.setScale({ scale, scale });
        s.setPosition({ 20.f, y });
        m_paletteIcons.push_back({ s, name });
        y += 80.f;
    }
}

void Editor::render() {
    for (auto& obj : m_sceneObjects) {
        bool isEditingThis = (m_selectedObject == &obj && m_isEditingCollider);
        obj.draw(m_window, &obj == m_selectedObject, isEditingThis);
    }

    sf::CircleShape startMarker(10.f);
    startMarker.setOrigin({ 10.f, 10.f });
    startMarker.setPosition(g_playerStartPos);
    startMarker.setFillColor(sf::Color::Cyan);
    m_window.draw(startMarker);

    drawSidebar();
    drawProperties();
    drawLevelSelector();
}

void Editor::drawSidebar() {
    m_window.draw(m_sidebarLeft);
    for (const auto& item : m_paletteIcons) {
        sf::Sprite s = item.first;
        s.move({ 0.f, -m_sidebarScroll });
        m_window.draw(s);
    }
}