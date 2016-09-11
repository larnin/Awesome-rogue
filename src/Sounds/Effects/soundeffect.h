#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H

#include "Utilities/noncopiable.h"
#include <SFML/Audio/Music.hpp>
#include <SFML/System/Time.hpp>

class SoundEffect : private NonCopiable
{
public:
    SoundEffect()
        : m_finished(false)
        , m_musicStopped(false)
        , m_volumModifier(1)
        , m_pitchModifier(1)
    {

    }

    SoundEffect(SoundEffect &&) = default;
    SoundEffect & operator =(SoundEffect &&) = default;
    virtual ~SoundEffect() = default;

    virtual void update(const sf::Time & elapsedTime) = 0;

    bool isFinished() const
    {
        return m_finished;
    }

    bool isMusicStopped() const
    {
        return m_musicStopped;
    }

    float getVolum() const
    {
        return m_volumModifier;
    }

    float getPitch() const
    {
        return m_pitchModifier;
    }

protected:
    bool m_finished;
    bool m_musicStopped;
    float m_volumModifier;
    float m_pitchModifier;
};

#endif // SOUNDEFFECT_H
