#ifndef COMMANDS_H
#define COMMANDS_H

#include "keyconfig.h"
#include "commandsvalue.h"
#include <SFML/Window/Event.hpp>

class Commands
{
public:
    Commands(const KeysConfig & config);

    void update();
    void event(const sf::Event & e);

    CommandsValue getValue() const;

private:
    KeysConfig m_config;
    CommandsValue m_values;
};

#endif // COMMANDS_H
