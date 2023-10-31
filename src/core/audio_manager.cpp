#include "audio_manager.h"

AudioManager::AudioManager()
{
    m_soundBufferMap[AudioType::JUMP] = sf::SoundBuffer();
    m_soundMap[AudioType::JUMP] = sf::Sound(m_soundBufferMap[AudioType::JUMP]);
    bool wasSfxLoaded = m_soundBufferMap[AudioType::JUMP].loadFromFile(JUMP_SFX_PATH);
    assert(wasSfxLoaded);
}

AudioManager::~AudioManager()
{
    for (sf::Music* music : m_sceneMusic)
    {
        delete music;
    }

    delete m_audioManager;
    m_sceneMusic.clear();
}

AudioManager* AudioManager::getInstance()
{
    if (m_audioManager != nullptr)
    {
        return m_audioManager;
    }

    m_audioManager = new AudioManager();
    currentSceneIndex = 0;

    return m_audioManager;
}

void AudioManager::playSound(AudioType actionType, float volume)
{
    sf::Sound& sound = m_soundMap[actionType];
    sound.setVolume(volume);
    sound.play();
}

void AudioManager::playMusic(uint8_t sceneIndex, float volume, bool shouldLoop)
{
    stopActiveMusic();

    sf::Music& nextMusic = *m_sceneMusic[sceneIndex];
    nextMusic.setVolume(volume);
    nextMusic.setLoop(shouldLoop);
    nextMusic.play();

    currentSceneIndex = sceneIndex;
}

bool AudioManager::isMusicPlaying()
{
    sf::Music& currentMusic = *m_sceneMusic[currentSceneIndex];
    return currentMusic.getStatus() == sf::SoundSource::Playing;
}

void AudioManager::stopActiveMusic()
{
    sf::Music& currentMusic = *m_sceneMusic[currentSceneIndex];
    currentMusic.stop();
}

sf::Time AudioManager::getCurrentMusicDuration()
{
    sf::Music& currentMusic = *m_sceneMusic[currentSceneIndex];
    return currentMusic.getDuration();
}