#ifndef KEYCONFIG_H
#define KEYCONFIG_H

#include "commandtype.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>
#include <array>

struct KeyInfo
{
    KeyInfo() = default;
    KeyInfo(sf::Keyboard::Key _key, unsigned int _button);
    KeyInfo(sf::Keyboard::Key _key, sf::Joystick::Axis _axis, bool _axisSide);

    sf::Keyboard::Key key;
    bool useAxis;
    unsigned int button;
    sf::Joystick::Axis axis;
    bool axisSide;
};

class KeysConfig
{
public:
    KeysConfig();
    KeysConfig(const std::string & filename);
    void save(const std::string & filename) const;

    std::array<KeyInfo, CommandType::COMMANDS_COUNT> commands;
    bool useJoystic;
    unsigned int joysticID;

    static const std::string keysFilename;
};

int getFirstConnectedJoystic();

#endif // KEYCONFIG_H
