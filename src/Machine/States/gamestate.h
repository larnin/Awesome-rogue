#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "state.h"
#include "Machine/gameholder.h"
#include "Events/eventreceiver.h"

class SimpleControler;
class EventInteraction;

class GameState : public State, public EventReceiver
{
public:
    GameState(std::weak_ptr<StateMachine> machine);
    GameState(GameState &&) = default;
    GameState & operator =(GameState &&) = default;
    virtual ~GameState() = default;

protected:
    virtual void enable();
    virtual void disable();

private:
    void onPressPause();
    void onPressMap();
    void onPressInventary();

    void onPortalTouched(EventInteraction e);

    GameHolder m_game;
    std::shared_ptr<SimpleControler> m_controler;
};

#endif // GAMESTATE_H
