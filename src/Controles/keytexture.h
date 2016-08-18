#ifndef KEYTEXTURE_H
#define KEYTEXTURE_H

#include "Utilities/ressource.h"
#include "keyconfig.h"

std::pair<Texture, Texture> texturesOfKey(const KeyInfo & key);

Texture keyTexture(sf::Keyboard::Key key);
std::string keyName(sf::Keyboard::Key key);

Texture joysticButtonTexture(unsigned int button);
Texture joysticAxisTexture(sf::Joystick::Axis axis, bool side);

Texture drawedText(const std::string & text);

#endif // KEYTEXTURE_H
