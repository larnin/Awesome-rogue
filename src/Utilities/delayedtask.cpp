#include "delayedtask.h"

DelayedTask::DelayedTask(std::function<void()> task, float time, bool start)
    : m_function(task)
    , m_life(time)
    , m_finished(false)
    , m_paused(!start)
{

}

void DelayedTask::pause()
{
    m_paused = true;
}

void DelayedTask::unPause()
{
    m_paused = false;
}

bool DelayedTask::finished() const
{
    return m_finished;
}

bool DelayedTask::paused() const
{
    return m_paused;
}

void DelayedTask::stop()
{
    m_finished = true;
}

void DelayedTask::restart(float time, bool start)
{
    m_finished = false;
    m_life = time;
    m_paused = !start;
}

void DelayedTask::update(const sf::Time & elapsedTime)
{
    if(finished() || paused())
        return;
    m_life -= elapsedTime.asSeconds();
    if(m_life <= 0)
    {
        m_finished = true;
        if(m_function)
            m_function();
    }
}
