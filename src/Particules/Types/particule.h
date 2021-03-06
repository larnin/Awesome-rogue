#ifndef PARTICULE_H
#define PARTICULE_H

#include "Systemes/updatable.h"
#include "Utilities/noncopiable.h"
#include "Map/location.h"
#include <SFML/Graphics/Drawable.hpp>
#include <memory>
#include <random>

class Particule : public sf::Drawable, public Updatable, private NonCopiable
{
public:
    Particule(const Location & pos);
    Particule(Particule &&) = default;
    Particule & operator=(Particule &&) = default;
    virtual ~Particule() = default;

    bool isKilled() const;
    Location getPos() const;

protected:
    Location m_pos;
    bool m_killed;

    static std::default_random_engine m_randEngine;
};

#endif // PARTICULE_H
