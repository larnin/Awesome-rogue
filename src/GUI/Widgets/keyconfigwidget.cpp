#include "keyconfigwidget.h"
#include "Controles/keyconfig.h"
#include "graphichelper.h"
#include "Controles/keytexture.h"
#include "Controles/commandsvalue.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

KeyConfigWidget::KeyConfigWidget(CommandType type, const KeyInfo & currentKey, const AdaptableBounds & _bounds)
    : Widget(_bounds)
    , m_key(currentKey)
    , m_type(type)
    , m_keyTexture(keyTexture(currentKey.key))
    , m_joyKeyTexture(currentKey.useAxis ? joysticAxisTexture(currentKey.axis, currentKey.axisSide)
                                         : joysticButtonTexture(currentKey.button))
    , m_waitInput(false)
{

}


void KeyConfigWidget::control(CommandsValue & v)
{
    Widget::control(v);

    if(!m_waitInput)
    {
        if(v.isPressEvent(CommandType::KEY_VALID))
            m_waitInput = true;
        return;
    }

    if(v.isPressEvent(CommandType::KEY_CANCEL))
    {
        m_waitInput = false;
        return;
    }

    bool keyChanged(false);
    bool joyKeyChanged(false);

    for(const auto & event : v.events)
    {
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code != sf::Keyboard::Unknown)
            {
                keyChanged = true;
                m_key.key = event.key.code;
            }
        }

        if(event.type == sf::Event::JoystickButtonPressed)
        {
            joyKeyChanged = true;
            m_key.useAxis = false;
            m_key.button = event.joystickButton.button;
        }

        if(event.type == sf::Event::JoystickMoved)
        {
            if(std::abs(event.joystickMove.position) > 50)
            {
                joyKeyChanged = true;
                m_key.useAxis = true;
                m_key.axis = event.joystickMove.axis;
                m_key.axisSide = event.joystickMove.position > 0;
            }
        }
    }

    if((keyChanged || joyKeyChanged))
    {
        m_waitInput = false;
        if(m_function)
            m_function(m_type, m_key);
        v.valideAll();
    }
    if(keyChanged)
        m_keyTexture = keyTexture(m_key.key);
    if(joyKeyChanged)
        m_joyKeyTexture = m_key.useAxis ? joysticAxisTexture(m_key.axis, m_key.axisSide)
                                        : joysticButtonTexture(m_key.button);
}

void KeyConfigWidget::update(const sf::Time & t)
{
    Widget::update(t);
    if(getState() == Controlable::UNACTIVE)
        m_waitInput = false;
}

void KeyConfigWidget::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::FloatRect b(bounds.boundsOnView(target.getView()));

    if(m_state == ControlState::ACTIVE)
        target.draw(drawSelector(b));

    sf::Sprite sKey(*m_keyTexture);
    sKey.setOrigin(sf::Vector2f(m_keyTexture->getSize())/2.0f);
    sKey.move(b.left+b.width/4, b.top+b.height/2);
    sKey.scale(0.5f, 0.5f);

    sf::Sprite sJoyKey(*m_joyKeyTexture);
    sJoyKey.setOrigin(sf::Vector2f(m_joyKeyTexture->getSize())/2.0f);
    sJoyKey.move(b.left+3*b.width/4, b.top+b.height/2);
    sJoyKey.scale(0.5f, 0.5f);

    if(m_waitInput)
    {
        sKey.setColor(sf::Color(255, 255, 255, 128));
        sJoyKey.setColor(sf::Color(255, 255, 255, 128));
    }

    target.draw(sKey);
    target.draw(sJoyKey);
}
