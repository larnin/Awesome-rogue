#include <cassert>
#include "entityfactory.h"
#include "Types/circlemob.h"
#include "Types/crosslasermob.h"
#include "Types/punchballmob.h"
#include "Types/squaremob.h"
#include "Types/trackermob.h"
#include "Events/event.h"
#include "Events/Datas/evententitycreated.h"

std::shared_ptr<Entity> EntityFactory::create(EntityType type, const Location & pos, bool sendEvent)
{
    std::shared_ptr<Entity> e;
    switch(type)
    {
    case E_CIRCLE_MOB:
        e = std::make_shared<CircleMob>(pos);
    break;
    case E_SMALL_CIRCLE_MOB:
        e = std::make_shared<CircleMob>(pos, true);
    break;
    case E_CROSS_LASER_MOB:
        e = std::make_shared<CrossLaserMob>(pos);
    break;
    case E_PUNCHBALL_MOB:
        e = std::make_shared<PunchBallMob>(pos);
    break;
    case E_SQUARE_MOB:
        e = std::make_shared<SquareMob>(pos);
    break;
    case E_TRACKER_MOB:
        e = std::make_shared<TrackerMob>(pos);
    break;
    default:
        assert(false);
    }

    if(sendEvent)
        Event<EventEntityCreated>::send(EventEntityCreated(e));
    return e;
}
