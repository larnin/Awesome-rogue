#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "state.h"
#include "Machine/gameholder.h"
#include "Events/eventreceiver.h"

class SimpleControler;
class EventInteraction;
class GenerationEnvironement;

class GameState : public State, public EventReceiver
{
public:
    GameState(std::weak_ptr<StateMachine> machine, const std::string & filename);
    GameState(std::weak_ptr<StateMachine> machine, const GenerationEnvironement & e);
    GameState(GameState &&) = default;
    GameState & operator =(GameState &&) = default;
    virtual ~GameState() = default;

protected:
    virtual void enable() override;
    virtual void disable() override;

private:
    void onPressPause();
    void onPressMap();
    void onPressInventary();

    void onSaveTouched(EventInteraction e);
    void onPortalTouched(EventInteraction e);

    void init();

    GameHolder m_game;
    std::shared_ptr<SimpleControler> m_controler;
};

#endif // GAMESTATE_H

