#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include "Events/eventreceiver.h"
#include "Utilities/noncopiable.h"
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <vector>
#include <memory>

class EventPlayMusic;
class EventPlaySound;
class EventStopMusic;
class SoundData;
class MusicData;
class DelayedTask;

class SoundPlayer : public EventReceiver, private NonCopiable
{
public:
    SoundPlayer();
    SoundPlayer(SoundPlayer &&) = default;
    SoundPlayer & operator=(SoundPlayer &&) = default;
    virtual ~SoundPlayer() = default;

    void playSound(const std::string & filename, const SoundData & data);
    void playMusic(const std::string & filename, const MusicData & data);
    void stopMusic();

    void setMusicVolum(float value);
    void setSoundVolum(float value);

private:

    struct SoundBufferData
    {
        SoundBufferData(const std::string & _filename);
        SoundBufferData(SoundBufferData &&) = default;
        SoundBufferData & operator =(SoundBufferData &&) = default;

        std::string filename;
        std::unique_ptr<sf::SoundBuffer> buffer;
        unsigned int useCount;
    };
    struct PlayedSoundData
    {
        PlayedSoundData(const sf::SoundBuffer & buffer, unsigned int _id, float _volum)
            : sound(buffer), id(_id), volum(_volum) {}

        sf::Sound sound;
        unsigned int id;
        float volum;
    };

    void onPlaySound(EventPlaySound e);
    void onPlayMusic(EventPlayMusic e);
    void onStopMusic(EventStopMusic);

    void delSound(unsigned int id);

    sf::Music m_music;
    float m_musicPlayedVolum;

    std::vector<SoundBufferData> m_buffers;
    std::vector<PlayedSoundData> m_playedSounds;
    std::vector<std::shared_ptr<DelayedTask>> m_delSoundDelay;

    float m_soundVolum;
    float m_musicVolum;
};

#endif // SOUNDPLAYER_H
