#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Controles/commands.h"
#include "Utilities/noncopiable.h"

class State;
class SmoothCamera;

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
    void changeFullScreen(bool value);
    void setWindowCenter(const sf::Vector2f & pos);
    void changeWindowZoom(float value);

    void setSubstate(std::unique_ptr<State> & sub);
    void resetSubstate();

private:
    std::unique_ptr<State> m_actualState;
    std::unique_ptr<State> m_nextState;
    std::unique_ptr<State> m_subState;

    sf::RenderWindow m_window;
    Commands m_commands;
    sf::Clock m_clock;
    sf::Color m_clearColor;
    std::shared_ptr<SmoothCamera> m_camera;
};

#endif // STATEMACHINE_H
