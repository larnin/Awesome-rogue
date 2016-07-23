#include "gamestate.h"
#include <SFML/Graphics/Color.hpp>
#include "Machine/statemachine.h"

GameState::GameState(std::weak_ptr<StateMachine> machine)
    : State(machine)
    , m_game(machine)
{

}

void GameState::enable()
{
    std::shared_ptr<StateMachine> mLock(m_machine.lock());
    mLock->setClearColor(sf::Color::Black);

    m_game.enable();
}

void GameState::disable()
{
    m_game.disable();
}
