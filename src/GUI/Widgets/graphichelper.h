#ifndef GRAPHICHELPER_H
#define GRAPHICHELPER_H

#include "GUI/aligntype.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Font.hpp>

class Margin;

sf::VertexArray drawSelector(sf::FloatRect rect);
sf::VertexArray drawButton(const sf::FloatRect & rect, const sf::FloatRect & texRect, const Margin & border);
sf::Texture drawText(const std::string & _text, const sf::Font & font, const sf::Vector2f & size, VAlign v, HAlign h);

#endif // GRAPHICHELPER_H
