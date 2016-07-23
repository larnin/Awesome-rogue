#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "state.h"
#include "Machine/gameholder.h"

class GameState : public State
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
    GameHolder m_game;
};

#endif // GAMESTATE_H
