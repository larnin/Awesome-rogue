#include "entityfactory.h"
#include "Types/entity.h"
#include "Types/circlemob.h"
#include "Types/crosslasermob.h"
#include "Types/punchballmob.h"
#include "Types/squaremob.h"
#include "Types/trackermob.h"
#include "Types/boss1part.h"
#include "Types/boss1end.h"
#include "Events/event.h"
#include "Events/Datas/evententitycreated.h"
#include "Map/location.h"
#include <cassert>

std::vector<std::shared_ptr<Entity> > EntityFactory::create(EntityType type, const Location & pos, bool sendEvent)
{
    std::vector<std::shared_ptr<Entity>> e;
    switch(type)
    {
    case E_CIRCLE_MOB:
        e.push_back(std::make_shared<CircleMob>(pos));
    break;
    case E_SMALL_CIRCLE_MOB:
        e.push_back(std::make_shared<CircleMob>(pos, true));
    break;
    case E_CROSS_LASER_MOB:
        e.push_back(std::make_shared<CrossLaserMob>(pos));
    break;
    case E_PUNCHBALL_MOB:
        e.push_back(std::make_shared<PunchBallMob>(pos));
    break;
    case E_SQUARE_MOB:
        e.push_back(std::make_shared<SquareMob>(pos));
    break;
    case E_TRACKER_MOB:
        e.push_back(std::make_shared<TrackerMob>(pos));
    break;
    case E_BOSS1_PARTS:
        e = createBoss1Parts(pos);
    break;
    case E_BOSS1_FINAL:
        e.push_back(std::make_shared<Boss1End>(pos));
    break;
    default:
        assert(false);
    }

    if(sendEvent)
        for(const auto m : e)
            Event<EventEntityCreated>::send(EventEntityCreated(m));
    return e;
}

std::vector<std::shared_ptr<Entity>> EntityFactory::createBoss1Parts(const Location & pos)
{
    const unsigned int nbParts(8);
    const float dist(7.0f);
    const float simpleAngle(3.14159f/(nbParts/2.0f));

    std::vector<std::shared_ptr<Entity>> parts;

    for(unsigned int i(0) ; i < nbParts ; i++)
        parts.push_back(std::make_shared<Boss1Part>(pos, simpleAngle*i, dist));
    return parts;
}
