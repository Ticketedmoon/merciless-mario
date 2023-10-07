#pragma once

#ifndef MERCILESS_MARIO_AUDIO_MANAGER_H
#define MERCILESS_MARIO_AUDIO_MANAGER_H

#include <SFML/Audio.hpp>

#include <cassert>

#include "action.h"

class AudioManager {
    public:
        enum class AudioType
        {
        };

        ~AudioManager();
        AudioManager(AudioManager& audioManager) = delete;
        void operator=(const AudioManager&) = delete;

        static AudioManager* getInstance();
        void playSound(AudioType audioType, float volume);
        void playMusic(uint8_t sceneIndex, float volume, bool shouldLoop);
        bool isMusicPlaying();
        void stopActiveMusic();
        sf::Time getCurrentMusicDuration();

    private:
        AudioManager();

    private:
        static inline AudioManager* m_audioManager;

        static inline std::unordered_map<AudioType, sf::SoundBuffer> m_soundBufferMap;
        static inline std::unordered_map<AudioType, sf::Sound> m_soundMap;
        static inline std::vector<sf::Music*> m_sceneMusic;
        static inline uint8_t currentSceneIndex;
};


#endif //MERCILESS_MARIO_AUDIO_MANAGER_H
