#ifndef COMMANDSVALUE_H
#define COMMANDSVALUE_H

#include "commandtype.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Event.hpp>
#include <array>
#include <vector>

class Commands;

class CommandsValue
{
    friend class Commands;
public:
    CommandsValue();
    bool isPressed(CommandType key) const;
    bool isPressEvent(CommandType key) const;
    bool isRelaseEvent(CommandType key) const;
    float getValue(CommandType key) const;
    void valide(CommandType key);
    void valideAll();

    std::vector<sf::Event> events;

private:
    std::array<bool, CommandType::COMMANDS_COUNT> m_pressedEventKey;
    std::array<bool, CommandType::COMMANDS_COUNT> m_relachedEventKey;
    std::array<float, CommandType::COMMANDS_COUNT> m_valueKey;
};

#endif // COMMANDSVALUE_H
