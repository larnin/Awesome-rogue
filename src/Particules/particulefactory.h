#ifndef PARTICULEFACTORY_H
#define PARTICULEFACTORY_H

#include "Types/particule.h"
#include "Events/event.h"
#include "Events/Datas/eventparticulecreated.h"
#include <memory>

class ParticuleFactory
{
public:
    template<typename T, typename... ARGS>
    static std::shared_ptr<Particule> create(ARGS &&... args)
    {
        return std::make_shared<T>(std::forward<ARGS>(args)...);
    }

    template<typename T, typename... ARGS>
    static std::shared_ptr<Particule> createSend(ARGS &&... args)
    {
        std::shared_ptr<Particule> p = std::make_shared<T>(std::forward<ARGS>(args)...);
        Event<EventParticuleCreated>::send(EventParticuleCreated(p));
        return p;
    }

private:
    ParticuleFactory() = delete;
};

#endif // PARTICULEFACTORY_H
