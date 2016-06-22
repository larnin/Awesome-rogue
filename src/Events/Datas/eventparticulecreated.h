#ifndef EVENTPARTICULECREATED_H
#define EVENTPARTICULECREATED_H

#include <memory>
#include "Particules/Types/particule.h"

struct EventParticuleCreated
{
    EventParticuleCreated(std::shared_ptr<Particule> p)
        : particule(p)
    {

    }

    std::shared_ptr<Particule> particule;
};

#endif // EVENTPARTICULECREATED_H
