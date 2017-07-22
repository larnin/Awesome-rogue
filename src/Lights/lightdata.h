#ifndef LIGHTDATA_H
#define LIGHTDATA_H

#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>

enum LightType
{
    POINT = 0,
    SPOT = 1,
    DIRECTIONNAL = 2,
};

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
