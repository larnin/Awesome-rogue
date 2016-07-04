#ifndef POPULATOR_H
#define POPULATOR_H

#include "Systemes/updatable.h"
#include "entitytype.h"
#include "Map/location.h"
#include "Events/eventreceiver.h"
#include "Events/Datas/eventpreplayerchangeroom.h"
#include <vector>
#include <random>

class Populator : public Updatable, public EventReceiver
{
public:
    Populator();
    virtual ~Populator() = default;

    void update(const sf::Time & elapsedTime);

private:
    void onPlayerChangeRoom(EventPrePlayerChangeRoom e);

    struct PopulatorData
    {
        PopulatorData(EntityType _type, const Location & _pos, float _time)
            : type(_type), pos(_pos), timeToSpawn(_time)
        { }

        EntityType type;
        Location pos;
        float timeToSpawn;
    };

    std::vector<PopulatorData> m_toSpawn;
    std::default_random_engine m_rand;
};

#endif // POPULATOR_H
