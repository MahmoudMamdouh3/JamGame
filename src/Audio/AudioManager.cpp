#include "AudioManager.h"
#include "../Core/Config.h" 
#include <iostream>
#include <cstdlib> 

AudioManager::AudioManager()
    : m_ambientTimer(10.0f)
{
}

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

    // SFML 3: Must construct Sound with the Buffer
    m_sounds.emplace_back(m_buffers[name]);
    m_sounds.back().play();
}

void AudioManager::playDialogue(const std::string& name) {
    if (m_buffers.find(name) == m_buffers.end()) {
        std::cerr << "Dialogue not found: " << name << std::endl;
        return;
    }

    // SFML 3 Fix: Re-create the sound with the new buffer
    m_voiceSource.emplace(m_buffers[name]);
    m_voiceSource->play();
}

void AudioManager::playMusic(const std::string& filepath) {
    if (m_music.openFromFile(filepath)) {
        // SFML 3 Fix: setLoop -> setLooping
        m_music.setLooping(true);
        m_music.setVolume(AUDIO_VOLUME);
        m_music.play();
    }
    else {
        std::cerr << "Failed to load music: " << filepath << std::endl;
    }
}

void AudioManager::stopMusic() {
    m_music.stop();
}

void AudioManager::setMusicVolume(float volume) {
    m_music.setVolume(volume);
}

void AudioManager::addAmbientSound(const std::string& name) {
    m_ambientKeys.push_back(name);
}

void AudioManager::update(float dt) {
    m_sounds.remove_if([](const sf::Sound& s) {
        return s.getStatus() == sf::Sound::Status::Stopped;
        });

    if (!m_ambientKeys.empty()) {
        m_ambientTimer -= dt;
        if (m_ambientTimer <= 0.0f) {
            int index = std::rand() % m_ambientKeys.size();
            playSound(m_ambientKeys[index]);

            float randomOffset = static_cast<float>(std::rand() % 1000) / 100.0f;
            m_ambientTimer = 25.0f + randomOffset;
        }
    }
}