#include "AudioManager.h"
#include "Config.h" // <--- ADD THIS INCLUDE
#include <iostream>

AudioManager::AudioManager() {}

void AudioManager::loadSound(const std::string& name, const std::string& filepath) {
    sf::SoundBuffer buffer;
    if (buffer.loadFromFile(filepath)) {
        m_buffers[name] = buffer;
    }
    else {
        std::cerr << "Failed to load sound: " << filepath << std::endl;
    }
}

void AudioManager::playSound(const std::string& name) {
    if (m_buffers.find(name) == m_buffers.end()) return;

    m_sounds.emplace_back(m_buffers[name]);
    m_sounds.back().play();
}

void AudioManager::playMusic(const std::string& filepath) {
    if (m_music.openFromFile(filepath)) {
        m_music.setLooping(true);
        m_music.setVolume(AUDIO_VOLUME); // Now this works because we included Config.h
        m_music.play();
    }
    else {
        std::cerr << "Failed to load music: " << filepath << std::endl;
    }
}

void AudioManager::setMusicVolume(float volume) {
    m_music.setVolume(volume);
}

void AudioManager::update() {
    m_sounds.remove_if([](const sf::Sound& s) {
        return s.getStatus() == sf::Sound::Status::Stopped;
        });
}