#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <list>

class AudioManager
{
private:
    // Sound Effects (RAM)
    std::map<std::string, sf::SoundBuffer> m_buffers;
    std::list<sf::Sound> m_sounds; // List to handle overlapping sounds

    // Music (Streamed)
    sf::Music m_music;

public:
    AudioManager();
    // Add this under the public section of AudioManager.h
    void setMusicVolume(float volume);

    // Load a sound effect (e.g., "jump", "assets/jump.wav")
    void loadSound(const std::string &name, const std::string &filepath);

    // Play a sound effect by name
    void playSound(const std::string &name);

    // Play background music
    void playMusic(const std::string &filepath);
    void stopMusic();

    // Clean up finished sounds to save memory
    void update();
};