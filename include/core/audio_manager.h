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
                JUMP,
                STOMP_ON_ENEMY,
                POWER_UP_APPEARS,
                BUMP,
                BREAK_BRICK,
                SHOTGUN_FIRE,
                SHOTGUN_RELOAD
        };

        ~AudioManager();
        AudioManager(AudioManager& audioManager) = delete;
        void operator=(const AudioManager&) = delete;

        static AudioManager* getInstance();
        static void playSound(AudioType audioType, float volume);
        static void playMusic(uint8_t sceneIndex, float volume, bool shouldLoop);
        static bool isMusicPlaying();
        static void stopActiveMusic();

    private:
        AudioManager();

        static void loadSfx(AudioType audioType, const std::string& sfxPath);

    private:
        static const inline std::string JUMP_SFX_PATH = "resources/assets/sound/sfx/mario_jump.ogg";
        static const inline std::string STOMP_ON_ENEMY_SFX = "resources/assets/sound/sfx/mario-enemy-stomp.ogg";
        static const inline std::string BUMP_SFX_PATH = "resources/assets/sound/sfx/mario_bump.ogg";
        static const inline std::string BREAK_BRICK_SFX_PATH = "resources/assets/sound/sfx/mario_breakbrick.ogg";
        static const inline std::string POWER_UP_APPEARS_SFX_PATH = "resources/assets/sound/sfx/mario_powerup_appears.ogg";
        static const inline std::string SHOTGUN_FIRE_APPEARS_SFX_PATH = "resources/assets/sound/sfx/shotgun-fire.ogg";
        static const inline std::string SHOTGUN_RELOAD_APPEARS_SFX_PATH = "resources/assets/sound/sfx/shotgun-reload.ogg";

        static const inline std::string LEVEL_ONE_MUSIC = "resources/assets/sound/music/1_1_music.ogg";

        static inline AudioManager* m_audioManager;

        static inline std::unordered_map<AudioType, sf::SoundBuffer> m_soundBufferMap;
        static inline std::unordered_map<AudioType, sf::Sound> m_soundMap;
        static inline std::vector<sf::Music*> m_sceneMusic;
        static inline uint8_t currentSceneIndex;
};


#endif //MERCILESS_MARIO_AUDIO_MANAGER_H
