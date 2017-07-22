#include "statemachine.h"
#include "States/state.h"
#include "Systemes/controlable.h"
#include "Systemes/drawablelist.h"
#include "Systemes/updatable.h"
#include "Events/Datas/Camera/eventsizeviewchanged.h"
#include "smoothcamera.h"
#include "Utilities/configs.h"
#include <SFML/Window/VideoMode.hpp>

const std::string windowTitle("Awesome rogue");

StateMachine::StateMachine(const KeysConfig & configs)
    : m_commands(configs)
    , m_clearColor(sf::Color::White)
{
    changeFullScreen(Configs::c.useFullScreen);

    m_camera = std::make_shared<SmoothCamera>(*this);
    m_camera->changeDefaultZoom(Configs::c.zoom);
    Updatable::add(m_camera);

    m_soundPlayer = std::make_shared<SoundPlayer>();
    Updatable::add(m_soundPlayer);
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

    m_window.setFramerateLimit(60);
    m_window.setKeyRepeatEnabled(false);
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
            if(m_subStates.empty() && m_actualState)
                m_actualState->disable();
            std::swap(m_actualState, m_nextState);
            m_nextState = std::unique_ptr<State>();
            if(m_subStates.empty() && m_actualState)
                m_actualState->enable();
        }

        m_commands.update();
        sf::Event e;
        while(m_window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                m_window.close();
            if(e.type == sf::Event::Resized)
                Event<EventSizeViewChanged>::send(EventSizeViewChanged(sf::Vector2f(e.size.width, e.size.height)));
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

void StateMachine::addSubstate(std::unique_ptr<State> & sub)
{
    if(!sub)
        return;
    if(!m_subStates.empty())
        m_subStates.top()->disable();
    else m_actualState->disable();

    m_subStates.push(std::move(sub));
    m_subStates.top()->enable();
}

void StateMachine::delSubstate()
{
    if(m_subStates.empty())
        return;
    m_subStates.top()->disable();
    m_subStates.pop();
    if(!m_subStates.empty())
        m_subStates.top()->enable();
    else m_actualState->enable();
}

void StateMachine::setMusicVolum(float value)
{
    m_soundPlayer->setMusicVolum(value);
}

void StateMachine::setSoundVolum(float value)
{
    m_soundPlayer->setSoundVolum(value);
}

bool StateMachine::isCurrentSubstate() const
{
    return !m_subStates.empty();
}
