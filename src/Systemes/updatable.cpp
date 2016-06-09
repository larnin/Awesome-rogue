#include <algorithm>
#include <memory>
#include "updatable.h"

#include <iostream>

Updatable::Updatable()
    : m_isAware(false)
{

}

bool Updatable::isAware()
{
    return m_isAware;
}

void Updatable::updateAll(const sf::Time & elapsedTime)
{
    applyModifications();

    for(auto & object : m_objects)
    {
        std::shared_ptr<Updatable> objectLock(object.lock());
        if(objectLock)
            objectLock->update(elapsedTime);
    }
    clean();
}

void Updatable::add(std::weak_ptr<Updatable> object)
{
    std::shared_ptr<Updatable> o(object.lock());
    if(o)
        o->awake();
    StaticList<Updatable>::add(object);
}

void Updatable::del(std::weak_ptr<Updatable> object)
{
    std::shared_ptr<Updatable> o(object.lock());
    if(o)
        o->disable();
    StaticList<Updatable>::del(object);
}

void Updatable::awake()
{
    if(m_isAware)
        return;
    onAwake();
    m_isAware = true;
}

void Updatable::disable()
{
    if(!m_isAware)
        return;
    onDisable();
    m_isAware = false;
}

void Updatable::onAwake()
{

}

void Updatable::onDisable()
{

}


