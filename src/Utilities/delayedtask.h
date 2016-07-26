#ifndef DELAYEDTASK_H
#define DELAYEDTASK_H

#include "Systemes/updatable.h"
#include <memory>
#include <functional>

class DelayedTask : public Updatable
{
public:
    virtual ~DelayedTask() = default;

    static std::weak_ptr<DelayedTask> create(std::function<void()> task, float time, bool start = true)
    {
        //std::shared_ptr<DelayedTask> t(std::make_shared<DelayedTask>(task, time));
        std::shared_ptr<DelayedTask> t(new DelayedTask(task, time));
        t->setThis(t);
        if(start)
            Updatable::add(t);

        return t;
    }

    void pause()
    {
        Updatable::del(m_this);
    }

    void unpause()
    {
        Updatable::add(m_this);
    }

    bool finished() const
    {
        return m_finished;
    }

    void stop()
    {
        onDisable();
    }

    virtual void update(const sf::Time & elapsedTime)
    {
        m_life -= elapsedTime.asSeconds();
        if(m_life <= 0 && !finished())
        {
            if(m_function)
                m_function();
            m_finished = true;
            m_this.reset();
        }
    }

protected:
    virtual void onDisable()
    {
        m_finished = true;
        m_this.reset();
    }

private:
    DelayedTask(std::function<void()> task, float time)
        : m_function(task)
        , m_life(time)
        , m_finished(false)
    {

    }

    void setThis(std::shared_ptr<DelayedTask> thisObj)
    {
        m_this = thisObj;
    }

    std::function<void()> m_function;
    std::shared_ptr<DelayedTask> m_this;
    float m_life;
    bool m_finished;
};

#endif // DELAYEDTASK_H
