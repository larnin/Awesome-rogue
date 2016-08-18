#include "gamestate.h"
#include <SFML/Graphics/Color.hpp>
#include "Machine/statemachine.h"
#include "GUI/simplecontroler.h"
#include "pausestate.h"

GameState::GameState(std::weak_ptr<StateMachine> machine)
    : State(machine)
    , m_game(machine)
{
    m_controler = std::make_shared<SimpleControler>();
    m_controler->set(CommandType::KEY_PAUSE, std::bind(&GameState::onPressPause, this));
    m_controler->set(CommandType::KEY_MAP, std::bind(&GameState::onPressMap, this));
    m_controler->set(CommandType::KEY_INVENTARY, std::bind(&GameState::onPressInventary, this));
}

void GameState::enable()
{
    std::shared_ptr<StateMachine> mLock(m_machine.lock());
    if(mLock)
        mLock->setClearColor(sf::Color::Black);

    m_game.enable();
    Controlable::add(m_controler);
}

void GameState::disable()
{
    m_game.disable();
    Controlable::del(m_controler);
}

void GameState::onPressPause()
{
    std::shared_ptr<StateMachine> mLock(m_machine.lock());
    if(mLock)
    {
        std::unique_ptr<State> s(std::make_unique<PauseState>(m_machine));
        mLock->setSubstate(s);
    }
}

void GameState::onPressMap()
{

}

void GameState::onPressInventary()
{

}
