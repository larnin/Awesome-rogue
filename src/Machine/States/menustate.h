#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <vector>
#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include "state.h"
#include "Utilities/ressource.h"

class Widget;

class MenuState : public State
{
public:
    MenuState(std::weak_ptr<StateMachine> machine);
    MenuState(MenuState&&) = default;
    MenuState & operator= (MenuState &&) = default;
    virtual ~MenuState();

private:
    void add(std::shared_ptr<Widget> w);
    void newGameFunction();
    void LoadFunction();
    void OptionsFunction();
    void exitFunction();

    std::shared_ptr<sf::Sprite> m_title;
    Texture m_titleTexture;
    std::vector<std::shared_ptr<Widget>> m_buttons;
};

#endif // MENUSTATE_H
