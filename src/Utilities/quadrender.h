#ifndef QUADRENDER_H
#define QUADRENDER_H

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Map/block.h"
#include "Collisions/hitbox.h"

void drawQuad(sf::Vertex* quads, const sf::FloatRect & rect, const sf::FloatRect & texRect
              , bool xFliped = false, bool yFliped = false, Rotation rot = Rotation::ROT_0);

void drawOrientedQuad(sf::Vertex* quads, const sf::FloatRect & rect, const sf::FloatRect & texRect, bool xFliped, bool yFliped, Rotation rot
                      , float rotation, const sf::Vector2f & rotationOrigine);

void drawFlatQuad(sf::Vertex* quads, const sf::FloatRect & rect, const sf::Color & color);

#endif // QUADRENDER_H
