#ifndef LIGHTSRENDER_H
#define LIGHTSRENDER_H

#include "Systemes/updatable.h"
#include "Map/room.h"
#include "Lights/lightdata.h"
#include <memory>

class LightsRender : public Updatable
{
public:
    LightsRender();
    virtual void update(const sf::Time & elapsedTime) override;
    void setRooms(const std::vector<std::weak_ptr<Room> > & rooms);
    inline void clear(){setRooms(std::vector<std::weak_ptr<Room>>());}

private:

    struct LightRenderData
    {
        LightRenderData(const AnimatedLightData & _animatedLight)
            : animatedLight(_animatedLight)
            , light(std::make_shared<LightData>(animatedLight.type()))
        {

        }

        const AnimatedLightData & animatedLight;
        std::shared_ptr<LightData> light;
    };

    float m_totalTime;
    std::vector<LightRenderData> m_lights;
};

#endif // LIGHTSRENDER_H
