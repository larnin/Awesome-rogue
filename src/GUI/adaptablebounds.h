#ifndef ADAPTABLEBOUNDS_H
#define ADAPTABLEBOUNDS_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "margin.h"

struct AdaptableBounds
{
    AdaptableBounds();
    AdaptableBounds(const sf::Vector2f & _pos
                    , const sf::Vector2f & _size
                    , const Margin & _margin
                    , VAlign _vAlign
                    , HAlign _hAlign);

    sf::FloatRect boundsOnView(const sf::View & v) const;

    sf::Vector2f pos;
    sf::Vector2f size;
    Margin margin;

    VAlign vAlign;
    HAlign hAlign;
};

#endif // ADAPTABLEBOUNDS_H
