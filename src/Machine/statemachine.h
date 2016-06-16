#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Controles/commands.h"
#include "Utilities/noncopiable.h"
#include "smoothcamera.h"

class State;

class StateMachine : private NonCopiable
{
public:
    StateMachine(const KeysConfig &configs);
    ~StateMachine() = default;
    sf::RenderWindow & getWindow();

    void setNext(std::unique_ptr<State> & state);
    void setKeys(const KeysConfig & configs);
    void run();
    void setClearColor(const sf::Color & color);
    void setWindowCenter(const sf::Vector2f & pos);

private:
    void resizeWindow(sf::Vector2u newSize);

    std::unique_ptr<State> m_actualState;
    std::unique_ptr<State> m_nextState;
    sf::RenderWindow m_window;
    Commands m_commands;
    sf::Clock m_clock;
    sf::Color m_clearColor;
    std::shared_ptr<SmoothCamera> m_camera;
};

#endif // STATEMACHINE_H
