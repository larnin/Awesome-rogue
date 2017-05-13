#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

struct PointLight
{
    inline PointLight()
        : color(sf::Color::White), pos(0, 0, 0), radius(100)
    { }

    inline PointLight(const sf::Color & c, const sf::Vector3f & p, float r)
        : color(c), pos(p), radius(r)
    { }

    inline PointLight(const sf::Color & c, const sf::Vector2f & p, float r)
        : color(c), pos(sf::Vector3f(p.x, p.y, 0)), radius(r)
    { }

    inline PointLight(const sf::Color &c, const sf::Vector2f &p, float h, float r)
        : color(c), pos(sf::Vector3f(p.x, p.y, h)), radius(r)
    { }

    inline void setPos(const sf::Vector2f & p) {pos = sf::Vector3f(p.x, p.y, pos.z);}
    inline void setPos(const sf::Vector2f & p, float h) {pos = sf::Vector3f(p.x, p.y, h);}
    inline void setHeight(float h) {pos.z = h;}

    inline void move(const sf::Vector2f & p) {pos += sf::Vector3f(p.x, p.y, 0);}
    inline void move(const sf::Vector2f & p, float h) {pos += sf::Vector3f(p.x, p.y, h);}
    inline void moveHeight(float h) {pos.z += h;}

    sf::Color color;
    sf::Vector3f pos;
    float radius;
};

#endif // POINTLIGHT_H
