#include "commandsvalue.h"
#include "keyconfig.h"
#include <cassert>

CommandsValue::CommandsValue()
{

}

bool CommandsValue::isPressed(CommandType key) const
{
    assert(key < CommandType::COMMANDS_COUNT);
    return m_valueKey[key] >= 0.5f;
}

bool CommandsValue::isPressEvent(CommandType key) const
{
    assert(key < CommandType::COMMANDS_COUNT);
    return m_pressedEventKey[key];
}

bool CommandsValue::isRelaseEvent(CommandType key) const
{
    assert(key < CommandType::COMMANDS_COUNT);
    return m_relachedEventKey[key];
}

float CommandsValue::getValue(CommandType key) const
{
    assert(key < CommandType::COMMANDS_COUNT);
    return m_valueKey[key];
}

void CommandsValue::valide(CommandType key)
{
    assert(key < CommandType::COMMANDS_COUNT);
    m_valueKey[key] = 0.0f;
    m_pressedEventKey[key] = false;
    m_relachedEventKey[key] = false;
}

void CommandsValue::valideAll()
{
    for(unsigned int i(0) ; i < CommandType::COMMANDS_COUNT ; i++)
        valide(CommandType(i));
}
