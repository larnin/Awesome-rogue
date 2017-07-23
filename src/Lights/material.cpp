#include "material.h"
#include <SFML/Graphics/Image.hpp>

Texture Material::defaultNormalTexture(generateDefaultNormalTexture());

Texture Material::generateDefaultNormalTexture()
{
    sf::Image i;
    i.create(4, 4, sf::Color(128, 128, 255));
    sf::Texture t;
    t.loadFromImage(i);
    return Texture(t);
}
