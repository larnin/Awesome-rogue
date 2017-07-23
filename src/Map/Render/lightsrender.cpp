#include "lightsrender.h"
#include "Events/event.h"
#include "Events/Datas/Light/eventaddlight.h"
#include "Events/Datas/Light/eventdellight.h"

LightsRender::LightsRender()
    : m_totalTime(0)
{

}

void LightsRender::update(const sf::Time & elapsedTime)
{
    m_totalTime += elapsedTime.asSeconds();
    for(auto & l : m_lights)
    {
        auto f(l.animatedLight.at(m_totalTime));
        l.light->pos = f.pos;
        l.light->color = f.color;
        l.light->radius = f.radius;
        l.light->yaw = f.yaw;
        l.light->pitch = f.pitch;
        l.light->intensity = f.intensity;
    }
}


void LightsRender::setRooms(const std::vector<std::weak_ptr<Room>> & rooms)
{
    for(const auto & l : m_lights)
        Event<EventDelLight>::send(EventDelLight(l.light));
    m_lights.clear();

    for(const auto & r : rooms)
    {
        const auto & rLock(r.lock());
        if(!rLock)
            continue;
        for(unsigned int i(0) ; i < rLock->lightCount() ; i++)
        {
            m_lights.push_back(LightRenderData(rLock->light(i)));
            Event<EventAddLight>::send(EventAddLight(m_lights.back().light));
        }
    }
}
