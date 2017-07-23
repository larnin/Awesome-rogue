#include "soundplayer.h"
#include "Events/Datas/Sound/eventplaymusic.h"
#include "Events/Datas/Sound/eventplaysound.h"
#include "Events/Datas/Sound/eventstopmusic.h"
#include "Utilities/delayedtask.h"
#include "sounddata.h"
#include "Systemes/updatable.h"
#include "Utilities/configs.h"
#include "Effects/fadeineffect.h"
#include "Effects/fadeouteffect.h"
#include <algorithm>

const unsigned int absoluteMaxSounds(254);

SoundPlayer::SoundPlayer()
    : m_soundVolum(Configs::c.soundsVolum)
    , m_musicVolum(Configs::c.musicVolum)
{
    connect<EventPlaySound>(std::bind(&onPlaySound, this, _1));
    connect<EventPlayMusic>(std::bind(&onPlayMusic, this, _1));
    connect<EventStopMusic>(std::bind(&onStopMusic, this, _1));
}

void SoundPlayer::update(const sf::Time &elapsedTime)
{
    for(auto & music : m_musics)
    {
        float volum(1);
        float pitch(1);
        for(auto & effect : music.effects)
        {
            effect->update(elapsedTime);
            volum *= effect->getVolum();
            pitch *= effect->getPitch();
        }
        music.music->setVolume(music.defaultVolum*volum*m_musicVolum);
        music.music->setPitch(music.defaultPitch*pitch);

        auto it(std::remove_if(music.effects.begin(), music.effects.end(), [](const auto & e){return e->isFinished();}));
        music.effects.erase(it, music.effects.end());
    }

    auto it(std::remove_if(m_musics.begin(), m_musics.end(), [](const auto & m){
        for(const auto & effect : m.effects)
            if(effect->isMusicStopped())
                return true;
        return false;
    }));

    m_musics.erase(it, m_musics.end());
}

void SoundPlayer::playSound(const std::string & filename, const SoundData & data)
{
    if(m_playedSounds.size() >= absoluteMaxSounds)
        return;

    auto it(std::find_if(m_buffers.begin(), m_buffers.end(), [filename](const auto & d){return d.filename == filename;}));
    if(it == m_buffers.end())
    {
        m_buffers.push_back(SoundBufferData(filename));
        it = m_buffers.end()-1;
    }
    it->useCount++;

    unsigned int id(m_playedSounds.empty() ? 0 : m_playedSounds.back().id+1);
    m_playedSounds.push_back(PlayedSoundData(*(it->buffer), id, data.volume));
    sf::Sound & s(*m_playedSounds.back().sound);
    s.setPitch(data.pitch);
    s.setVolume(data.volume*m_soundVolum);
    s.play();

    m_delSoundDelay.push_back(std::make_shared<DelayedTask>(std::bind(&delSound, this, id), it->buffer->getDuration().asSeconds()/data.pitch+0.5f));
    Updatable::add(m_delSoundDelay.back());

}

void SoundPlayer::playMusic(const std::string & filename, const MusicData & data, float fadeout, float fadein, float delay)
{
    for(auto & music : m_musics)
        music.effects.push_back(std::make_unique<FadeoutEffect>(0, fadeout));
    m_musics.push_back(PlayedMusicData(filename, data.volume, data.pitch, data.loop));
    m_musics.back().effects.push_back(std::make_unique<FadeinEffect>(delay, fadein));
    m_musics.back().music->play();
}

void SoundPlayer::stopMusic(float fadeout)
{
    for(auto & music : m_musics)
        music.effects.push_back(std::make_unique<FadeoutEffect>(0, fadeout));
}

void SoundPlayer::onPlaySound(EventPlaySound e)
{
    playSound(e.filename, e.data);
}

void SoundPlayer::onPlayMusic(EventPlayMusic e)
{
    playMusic(e.filename, e.data, e.fadeoutTime, e.fadeinTime, e.delay);
}

void SoundPlayer::onStopMusic(EventStopMusic e)
{
    stopMusic(e.fadeoutTime);
}

SoundPlayer::SoundBufferData::SoundBufferData(const std::string & _filename)
    : filename(_filename)
    , useCount(0)
{
    buffer = std::make_unique<sf::SoundBuffer>();
    if(!buffer->loadFromFile(filename))
        throw std::invalid_argument("File \"" + filename + "\" not found !");
}

void SoundPlayer::delSound(unsigned int id)
{
    auto itSound(std::find_if(m_playedSounds.begin(), m_playedSounds.end(), [id](const auto & s){return s.id == id;}));
    assert(itSound != m_playedSounds.end());

    auto itBuffer(std::find_if(m_buffers.begin(), m_buffers.end(), [itSound](const auto & b){return itSound->sound->getBuffer() == b.buffer.get();}));
    assert(itBuffer != m_buffers.end());

    m_playedSounds.erase(itSound);
    itBuffer->useCount --;
    if(itBuffer->useCount == 0)
        m_buffers.erase(itBuffer);

    auto itDel(std::remove_if(m_delSoundDelay.begin(), m_delSoundDelay.end(), [](const auto & d){return d->finished();}));
    m_delSoundDelay.erase(itDel, m_delSoundDelay.end());
}

void SoundPlayer::setMusicVolum(float value)
{
    m_musicVolum = value;
}

void SoundPlayer::setSoundVolum(float value)
{
    m_soundVolum = value;
    for(auto & v : m_playedSounds)
        v.sound->setVolume(m_soundVolum*v.volum);
}
