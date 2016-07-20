#include <SFML/Window/VideoMode.hpp>
#include "statemachine.h"
#include "States/state.h"

#include "Systemes/controlable.h"
#include "Systemes/drawablelist.h"
#include "Systemes/updatable.h"

#include "Events/Datas/eventsizeviewchanged.h"
#include "smoothcamera.h"

const unsigned int windowWidth(1200);
const unsigned int windowHeight(900);
const std::string windowTitle("Awesome rogue");

StateMachine::StateMachine(const KeysConfig & configs)
    : m_window(sf::VideoMode(windowWidth, windowHeight), windowTitle, sf::Style::Default, sf::ContextSettings(0, 0, 2))
    , m_commands(configs)
    , m_clearColor(sf::Color::White)
{
    m_window.setFramerateLimit(60);

    m_camera = std::make_shared<SmoothCamera>(*this);
    Updatable::add(m_camera);
}

sf::RenderWindow & StateMachine::getWindow()
{
    return m_window;
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

void StateMachine::run()
{
    if(!m_actualState && !m_nextState)
        return;

    m_clock.restart();

    while(m_window.isOpen())
    {
        if(m_nextState)
        {
            std::swap(m_actualState, m_nextState);
            //m_nextState.reset();
            m_nextState = std::unique_ptr<State>();
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
}

void StateMachine::setWindowCenter(const sf::Vector2f & pos)
{
    sf::View v(m_window.getView());
    v.setCenter(sf::Vector2f(sf::Vector2i(pos)));
    m_window.setView(v);
}
