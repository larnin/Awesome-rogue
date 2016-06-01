#include "commands.h"

const float maxAxisValue(100.0f);
const float pressThreshold(0.6f);
const float relaseThreshold(0.4f);

Commands::Commands(const KeysConfig &config)
    : m_config(config)
{

}

CommandsValue Commands::getValue() const
{
    return m_values;
}

void Commands::update()
{
    m_values.events.clear();

    for(bool & p : m_values.m_pressedEventKey)
        p = false;
    for(bool & r : m_values.m_relachedEventKey)
        r = false;

    for(unsigned int keyID(0) ; keyID < CommandType::COMMANDS_COUNT ; keyID++)
    {
        if(sf::Keyboard::isKeyPressed(m_config.commands[keyID].key))
        {
            m_values.m_valueKey[keyID] = 1.0f;
            continue;
        }
        else if(m_config.useJoystic && sf::Joystick::isConnected(m_config.joysticID))
        {
            if(m_config.commands[keyID].useAxis)
            {
                float axeValue(sf::Joystick::getAxisPosition(m_config.joysticID, m_config.commands[keyID].axis)/maxAxisValue);
                if(!m_config.commands[keyID].axisSide)
                    axeValue *= -1.0f;
                if(axeValue < 0.0f)
                    axeValue = 0.0f;

                if(axeValue > pressThreshold && m_values.m_valueKey[keyID] <= pressThreshold)
                    m_values.m_pressedEventKey[keyID] = true;
                if(axeValue < relaseThreshold && m_values.m_valueKey[keyID] >= relaseThreshold)
                    m_values.m_relachedEventKey[keyID] = true;
                m_values.m_valueKey[keyID] = axeValue;
            }
            else if(sf::Joystick::isButtonPressed(m_config.joysticID, m_config.commands[keyID].button))
                     m_values.m_valueKey[keyID] = 1.0f;
        }
        else m_values.m_valueKey[keyID] = 0.0f;
    }
}

void Commands::event(const sf::Event & e)
{
    for(unsigned int keyID(0) ; keyID < CommandType::COMMANDS_COUNT ; keyID++)
    {
        switch(e.type)
        {
        case sf::Event::KeyPressed:
            if(e.key.code == m_config.commands[keyID].key)
                m_values.m_pressedEventKey[keyID] = true;
            break;
        case sf::Event::KeyReleased:
            if(e.key.code == m_config.commands[keyID].key)
                m_values.m_relachedEventKey[keyID] = true;
            break;
        case sf::Event::JoystickButtonPressed:
            if(e.joystickButton.joystickId == m_config.joysticID
                    && e.joystickButton.button == m_config.commands[keyID].button
                    && !m_config.commands[keyID].useAxis)
                m_values.m_pressedEventKey[keyID] = true;
            break;
        case sf::Event::JoystickButtonReleased:
            if(e.joystickButton.joystickId == m_config.joysticID
                    && e.joystickButton.button == m_config.commands[keyID].button
                    && !m_config.commands[keyID].useAxis)
                m_values.m_relachedEventKey[keyID] = true;
            break;
        default:
            break;
        }
    }

    m_values.events.push_back(e);
}
