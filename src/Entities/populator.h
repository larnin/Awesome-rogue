#ifndef POPULATOR_H
#define POPULATOR_H

#include "entitytype.h"
#include "Map/location.h"
#include "Events/eventreceiver.h"
#include <vector>
#include <random>

class EventPrePlayerChangeRoom;
class EventInteraction;
class DelayedTask;

class Populator : public EventReceiver
{
public:
    Populator();
    virtual ~Populator() = default;

    void enable();
    void disable();

private:
    void cleanTasks();

    void onPlayerChangeRoom(EventPrePlayerChangeRoom e);
    void onSpawnBossInteraction(EventInteraction e);

    void startBoss1(const Location & pos);
    void startBoss1Final(const Location & pos);

    std::default_random_engine m_rand;

    std::vector<std::shared_ptr<DelayedTask>> m_tasks;

    bool m_enabled;
};

#endif // POPULATOR_H
