#ifndef UPDATABLE_H
#define UPDATABLE_H

#include <SFML/System/Time.hpp>

#include "staticlist.h"

class Updatable : protected StaticList<Updatable>
{
public:
    Updatable();
    virtual ~Updatable() = default;

    bool isAware();

    void awake();
    void disable();
    virtual void update(const sf::Time & elapsedTime) = 0;

    static void updateAll(const sf::Time & elapsedTime);

    static void add(std::weak_ptr<Updatable> object);
    static void del(std::weak_ptr<Updatable> object);
    using StaticList<Updatable>::clean;

protected:
    virtual void onAwake();
    virtual void onDisable();

private:
    bool m_isAware;
};

#endif // UPDATABLE_H
