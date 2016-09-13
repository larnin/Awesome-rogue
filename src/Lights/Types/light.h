#ifndef LIGHT_H
#define LIGHT_H

#include "Utilities/noncopiable.h"
#include <SFML/Graphics/RenderTarget.hpp>

class Light : private NonCopiable
{
public:
    Light(const sf::Vector2f & pos)
        : m_pos(pos) {}
    Light(Light &&) = default;
    Light & operator =(Light &&) = default;
    virtual ~Light() = default;

    virtual void drawBase(sf::RenderTarget & target) const = 0;
    virtual void drawSaturation(sf::RenderTarget & target) const = 0;
    void move(const sf::Vector2f & dir)
    {
        m_pos += dir;
    }

    void setPos(const sf::Vector2f & pos)
    {
        m_pos = pos;
    }

protected:
    sf::Vector2f m_pos;
};

#endif // LIGHT_H
