#ifndef LIGHTDATA_H
#define LIGHTDATA_H

#include <SFML/System/Vector3.hpp>

enum LightType
{
    POINT = 0,
    SPOT = 1,
    DIRECTIONNAL = 2,
};

struct LightData
{
    inline LightData();

    float time;
    sf::Vector3f pos;
    sf::Color color;
    float radius;
    float yaw;
    float pitch; //or angle for spot type
    float intensity;
    LightType type;
};

#endif // LIGHTDATA_H
