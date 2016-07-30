#include "animation.h"
#include <cassert>

Animation::Animation(bool singleShoot)
    : m_singleShoot(singleShoot)
    , m_finished(true)
    , m_currentTime(0)
    , m_totalTime(0)
{

}

void Animation::addFrame(const Frame & f)
{
    m_frames.push_back(f);
    m_totalTime+= f.time;
    m_finished = false;
}

bool Animation::isSingleShoot() const
{
    return m_singleShoot;
}

void Animation::setSingleShoot(bool value)
{
    m_singleShoot = value;
}

bool Animation::finished() const
{
    return m_finished;
}

void Animation::restart()
{
    if(m_frames.empty())
        return;
    m_finished = false;
    m_currentTime = 0;
}

Frame Animation::getCurrent() const
{
    return getAt(m_currentTime);
}

Frame Animation::getAt(float time) const
{
    assert(time >= 0);

    if(time >= m_totalTime)
    {
        if(m_singleShoot)
        {
            Frame f(m_frames.back());
            f.time = 0;
            return f;
        }

        time = fmod(time, m_totalTime);
    }

    for(const Frame & f : m_frames)
    {
        time -= f.time;
        if(time < 0)
        {
            Frame fReturn(f);
            fReturn.time = -time;
            return fReturn;
        }
    }
    assert(false);
    return Frame();
}

void Animation::setTime(float time)
{
    assert(time >= 0);

    if(time > m_totalTime)
    {
        if(m_singleShoot)
        {
            m_finished = true;
            m_currentTime = m_totalTime;
            return;
        }
        time = fmod(time, m_totalTime);
    }
    m_currentTime = time;
}

void Animation::addTime(float time)
{
    assert(time >= 0);
    setTime(m_currentTime+time);
}

float Animation::getCurrentTime() const
{
    return m_currentTime;
}

float Animation::getAnimationTime() const
{
    return m_totalTime;
}
