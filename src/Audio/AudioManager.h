#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <optional> // Added for SFML 3 compatibility

class AudioManager
{
private:
    // Sound Effects (RAM)
    std::map<std::string, sf::SoundBuffer> m_buffers;
    std::list<sf::Sound> m_sounds;

    // Voice / Dialogue (RAM)
    // SFML 3 Fix: Wrapped in optional because sf::Sound now requires a buffer at construction
    std::optional<sf::Sound> m_voiceSource;

    // Music (Streamed)
    sf::Music m_music;

    // Ambient System
    std::vector<std::string> m_ambientKeys;
    float m_ambientTimer;

public:
    AudioManager();

    void setMusicVolume(float volume);
    void loadSound(const std::string& name, const std::string& filepath);
    void playSound(const std::string& name);
    void playMusic(const std::string& filepath);
    void stopMusic();
    void playDialogue(const std::string& name);
    void addAmbientSound(const std::string& name);
    void update(float dt);
};