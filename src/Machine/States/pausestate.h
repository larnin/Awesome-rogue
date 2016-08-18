#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include "state.h"
#include <SFML/Graphics/Sprite.hpp>
#include "Utilities/ressource.h"

class Widget;

class PauseState : public State
{
public:
    PauseState(std::weak_ptr<StateMachine> machine);
    PauseState(PauseState &&) = default;
    PauseState & operator =(PauseState &&) = default;
    virtual ~PauseState() = default;

protected:
    virtual void enable();
    virtual void disable();

private:
    void continueFunction();
    void mapFunction();
    void inventaryFunction();
    void optionsFunction();
    void quitFunction();

    std::shared_ptr<sf::Sprite> m_title;
    Texture m_titleTexture;
    std::vector<std::shared_ptr<Widget>> m_buttons;

};

#endif // PAUSESTATE_H
