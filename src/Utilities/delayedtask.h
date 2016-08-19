#ifndef DELAYEDTASK_H
#define DELAYEDTASK_H

#include "Systemes/updatable.h"
#include "Utilities/noncopiable.h"
#include <memory>
#include <functional>

class DelayedTask : public Updatable, private NonCopiable
{
public:
    DelayedTask(std::function<void()> task, float time, bool start = true);
    DelayedTask(DelayedTask &&) = default;
    DelayedTask & operator =(DelayedTask &&) = default;
    virtual ~DelayedTask() = default;

    void pause();
    void unPause();
    bool finished() const;
    bool paused() const;
    void stop();
    void restart(float time, bool start = true);
    virtual void update(const sf::Time & elapsedTime);

private:
    std::function<void()> m_function;
    float m_life;
    bool m_finished;
    bool m_paused;
};


#endif // DELAYEDTASK_H
