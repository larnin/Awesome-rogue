#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>
#include <stack>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Controles/commands.h"
#include "Utilities/noncopiable.h"
#include "Sounds/soundplayer.h"

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

    void addSubstate(std::unique_ptr<State> & sub);
    void delSubstate();

    void setMusicVolum(float value);
    void setSoundVolum(float value);

private:
    std::unique_ptr<State> m_actualState;
    std::unique_ptr<State> m_nextState;
    std::stack<std::unique_ptr<State>> m_subStates;

    sf::RenderWindow m_window;
    Commands m_commands;
    sf::Clock m_clock;
    sf::Color m_clearColor;
    std::shared_ptr<SmoothCamera> m_camera;
    SoundPlayer m_soundPlayer;
};

#endif // STATEMACHINE_H
