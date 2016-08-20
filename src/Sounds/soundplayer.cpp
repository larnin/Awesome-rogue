#include "soundplayer.h"
#include "Events/Datas/eventplaymusic.h"
#include "Events/Datas/eventplaysound.h"
#include "Events/Datas/eventstopmusic.h"
#include "Utilities/delayedtask.h"
#include "sounddata.h"
#include "Systemes/updatable.h"
#include <algorithm>
#include "Utilities/configs.h"

const unsigned int absoluteMaxSounds(254);

SoundPlayer::SoundPlayer()
    : m_soundVolum(Configs::c.soundsVolum)
    , m_musicVolum(Configs::c.musicVolum)
{
    connect<EventPlaySound>(std::bind(&onPlaySound, this, _1));
    connect<EventPlayMusic>(std::bind(&onPlayMusic, this, _1));
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
    sf::Sound & s(m_playedSounds.back().sound);
    s.setPitch(data.pitch);
    s.setVolume(data.volume*m_soundVolum);
    s.play();

    m_delSoundDelay.push_back(std::make_shared<DelayedTask>(std::bind(&delSound, this, id), it->buffer->getDuration().asSeconds()/data.pitch+0.5f));
    Updatable::add(m_delSoundDelay.back());

}

void SoundPlayer::playMusic(const std::string & filename, const MusicData & data)
{
    if(!m_music.openFromFile(filename))
        throw std::invalid_argument("File \"" + filename + "\" not found !");
    m_musicPlayedVolum = data.volume;
    m_music.setAttenuation(0);;
    m_music.setLoop(data.loop);
    m_music.setPitch(data.pitch);
    m_music.setVolume(data.volume*m_musicVolum);
    m_music.play();
}

void SoundPlayer::stopMusic()
{
    m_music.stop();
}

void SoundPlayer::onPlaySound(EventPlaySound e)
{
    playSound(e.filename, e.data);
}

void SoundPlayer::onPlayMusic(EventPlayMusic e)
{
    playMusic(e.filename, e.data);
}

void SoundPlayer::onStopMusic(EventStopMusic)
{
    stopMusic();
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

    auto itBuffer(std::find_if(m_buffers.begin(), m_buffers.end(), [itSound](const auto & b){return itSound->sound.getBuffer() == b.buffer.get();}));
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
    m_music.setVolume(m_musicVolum*m_musicPlayedVolum);
}

void SoundPlayer::setSoundVolum(float value)
{
    m_soundVolum = value;
    for(auto & v : m_playedSounds)
        v.sound.setVolume(m_soundVolum*v.volum);
}
