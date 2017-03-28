#include "gamestate.h"
#include "Machine/statemachine.h"
#include "GUI/simplecontroler.h"
#include "pausestate.h"
#include "mapstate.h"
#include "loadsavestate.h"
#include "Events/Datas/eventinteraction.h"
#include "Events/event.h"
#include "File/serializer.h"
#include <SFML/Graphics/Color.hpp>

GameState::GameState(std::weak_ptr<StateMachine> machine, const std::string & filename)
    : State(machine)
    , m_game(machine, filename)
{
    init();
}

GameState::GameState(std::weak_ptr<StateMachine> machine, const GenerationEnvironement & e)
    : State(machine)
    , m_game(machine, e)
{
   init();
}

void GameState::init()
{
    m_controler = std::make_shared<SimpleControler>();
    m_controler->set(CommandType::KEY_PAUSE, std::bind(&GameState::onPressPause, this));
    m_controler->set(CommandType::KEY_MAP, std::bind(&GameState::onPressMap, this));
    m_controler->set(CommandType::KEY_INVENTARY, std::bind(&GameState::onPressInventary, this));

    connect<EventInteraction>(std::bind(&onPortalTouched, this, _1));
    connect<EventInteraction>(std::bind(&onSaveTouched, this, _1));
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
        mLock->addSubstate(s);
    }
}

void GameState::onPressMap()
{
    std::shared_ptr<StateMachine> mLock(m_machine.lock());
    if(mLock)
    {
        std::unique_ptr<State> s(std::make_unique<MapState>(m_machine));
        mLock->addSubstate(s);
    }
}

void GameState::onPressInventary()
{

}

void GameState::onPortalTouched(EventInteraction e)
{
    if(e.type != BlockInteractionType::BI_PORTAL)
        return;

    std::shared_ptr<StateMachine> mLock(m_machine.lock());
    if(mLock)
    {
        std::unique_ptr<State> s(std::make_unique<MapState>(m_machine, true));
        mLock->addSubstate(s);
    }
}

void GameState::onSaveTouched(EventInteraction e)
{
    if(e.type != BlockInteractionType::BI_SAVEPOINT)
        return;

    std::shared_ptr<StateMachine> mLock(m_machine.lock());
    if(mLock)
    {
        std::unique_ptr<State> s(std::make_unique<LoadSaveState>(m_machine, false));
        mLock->addSubstate(s);
    }
}
