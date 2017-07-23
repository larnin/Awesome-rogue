#ifndef LIGHTDATA_H
#define LIGHTDATA_H

#include "lighttype.h"
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>

struct LightData
{
    inline LightData(LightType _type)
        : type(_type)
        , pos(0, 0, 0)
        , color(sf::Color::White)
        , radius(100)
        , yaw(0)
        , pitch(0)
        , intensity(1)
    {

    }

    LightType type;
    sf::Vector3f pos;
    sf::Color color;
    float radius;
    float yaw;
    float pitch; //or angle for spot type
    float intensity;
};

#endif // LIGHTDATA_H
