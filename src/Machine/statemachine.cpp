#include <SFML/Window/VideoMode.hpp>
#include "statemachine.h"
#include "States/state.h"

#include "Systemes/controlable.h"
#include "Systemes/drawablelist.h"
#include "Systemes/updatable.h"

#include "Events/Datas/eventsizeviewchanged.h"
#include "smoothcamera.h"

#include "Utilities/configs.h"

const std::string windowTitle("Awesome rogue");

StateMachine::StateMachine(const KeysConfig & configs)
    : m_commands(configs)
    , m_clearColor(sf::Color::White)
{
    if(Configs::c.useFullScreen)
        m_window.create(sf::VideoMode::getFullscreenModes().front(), windowTitle, sf::Style::Fullscreen, sf::ContextSettings(0, 0, 2));
    else m_window.create(sf::VideoMode(Configs::c.screenSize.x, Configs::c.screenSize.y), windowTitle, sf::Style::Default, sf::ContextSettings(0, 0, 2));

    m_window.setFramerateLimit(60);
    m_window.setKeyRepeatEnabled(false);

    m_camera = std::make_shared<SmoothCamera>(*this);
    m_camera->changeDefaultZoom(Configs::c.zoom);
    Updatable::add(m_camera);
}

sf::RenderWindow & StateMachine::getWindow()
{
    return m_window;
}

void StateMachine::changeFullScreen(bool value)
{
    if(value)
        m_window.create(sf::VideoMode::getFullscreenModes().front(), windowTitle, sf::Style::Fullscreen, sf::ContextSettings(0, 0, 2));
    else m_window.create(sf::VideoMode(Configs::c.screenSize.x, Configs::c.screenSize.y), windowTitle, sf::Style::Default, sf::ContextSettings(0, 0, 2));
}

void StateMachine::setNext(std::unique_ptr<State> & state)
{
    m_nextState = std::move(state);
}

void StateMachine::setKeys(const KeysConfig & configs)
{
    m_commands = Commands(configs);
}

void StateMachine::setClearColor(const sf::Color & color)
{
    m_clearColor = color;
}

void StateMachine::changeWindowZoom(float value)
{
    m_camera->changeDefaultZoom(value);
}

void StateMachine::run()
{
    if(!m_actualState && !m_nextState)
        return;

    m_clock.restart();

    while(m_window.isOpen())
    {
        if(m_nextState)
        {
            if(!m_subState && m_actualState)
                m_actualState->disable();
            std::swap(m_actualState, m_nextState);
            m_nextState = std::unique_ptr<State>();
            if(!m_subState && m_actualState)
                m_actualState->enable();
        }

        m_commands.update();
        sf::Event e;
        while(m_window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                m_window.close();
            m_commands.event(e);
        }

        Controlable::update(m_commands.getValue());
        Updatable::updateAll(m_clock.restart());

        m_window.clear(m_clearColor);
        DrawableList::drawAll(m_window);
        m_window.display();
    }

    if(!Configs::c.useFullScreen)
        Configs::c.screenSize = m_window.getSize();
}

void StateMachine::setWindowCenter(const sf::Vector2f & pos)
{
    sf::View v(m_window.getView());
    v.setCenter(sf::Vector2f(sf::Vector2i(pos)));
    m_window.setView(v);
}

void StateMachine::setSubstate(std::unique_ptr<State> & sub)
{
    if(m_subState)
        m_subState->disable();
    m_subState = std::move(sub);
    if(m_subState)
        m_subState->enable();
    if(m_subState)
    {
        if(m_actualState)
            m_actualState->disable();
    }
    else
    {
        if(m_actualState)
            m_actualState->enable();
    }
}

void StateMachine::resetSubstate()
{
    std::unique_ptr<State> s;
    setSubstate(s);
}
