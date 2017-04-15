#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include "Events/eventreceiver.h"
#include "Utilities/noncopiable.h"
#include "Systemes/updatable.h"
#include "Effects/soundeffect.h"
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

class SoundPlayer : public EventReceiver, public Updatable, private NonCopiable
{
public:
    SoundPlayer();
    SoundPlayer(SoundPlayer &&) = default;
    SoundPlayer & operator=(SoundPlayer &&) = default;
    virtual ~SoundPlayer() = default;

    virtual void update(const sf::Time &elapsedTime) override;

    void playSound(const std::string & filename, const SoundData & data);
    void playMusic(const std::string &filename, const MusicData &data, float fadeout, float fadein, float delay);
    void playMusic(const std::string & filename, const MusicData & data, float fadeout = 0, float fadein = 0)
    { playMusic(filename, data, fadeout, fadein); }
    void stopMusic(float fadeout = 0);

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
            : id(_id), volum(_volum)
        {
            sound = std::make_unique<sf::Sound>(buffer);
        }

        std::unique_ptr<sf::Sound> sound;
        unsigned int id;
        float volum;
    };
    struct PlayedMusicData
    {
        PlayedMusicData(const std::string & filename, float volum, float pitch, bool loop)
            : defaultVolum(volum)
            , defaultPitch(pitch)
        {
            music = std::make_unique<sf::Music>();
            music->openFromFile(filename);
            music->setLoop(loop);
            music->setVolume(defaultVolum);
            music->setPitch(defaultPitch);
        }

        PlayedMusicData(PlayedMusicData &&) = default;
        PlayedMusicData & operator =(PlayedMusicData &&) = default;

        float defaultVolum;
        float defaultPitch;

        std::unique_ptr<sf::Music> music;
        std::vector<std::unique_ptr<SoundEffect>> effects;
    };

    void onPlaySound(EventPlaySound e);
    void onPlayMusic(EventPlayMusic e);
    void onStopMusic(EventStopMusic e);

    void delSound(unsigned int id);

    std::vector<PlayedMusicData> m_musics;

    std::vector<SoundBufferData> m_buffers;
    std::vector<PlayedSoundData> m_playedSounds;
    std::vector<std::shared_ptr<DelayedTask>> m_delSoundDelay;

    float m_soundVolum;
    float m_musicVolum;
};

#endif // SOUNDPLAYER_H
