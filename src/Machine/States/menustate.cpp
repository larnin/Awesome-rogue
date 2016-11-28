#include "menustate.h"
#include "Events/event.h"
#include "Events/Datas/eventinstantcenterofviewchanged.h"
#include "Events/Datas/eventplaycameraeffect.h"
#include "Systemes/drawablelist.h"
#include "Machine/statemachine.h"
#include "GUI/Widgets/Buttons/basicbutton.h"
#include "Machine/States/gamestate.h"
#include "Machine/States/optionsstate.h"
#include "Machine/States/loadsavestate.h"
#include "GUI/Widgets/widget.h"
#include "Utilities/tr.h"

#include "Events/Datas/eventplaymusic.h"

MenuState::MenuState(std::weak_ptr<StateMachine> machine)
    : State(machine)
    , m_titleTexture("res/img/title.png")
{
    m_title = std::make_shared<sf::Sprite>(*m_titleTexture);
    m_title->setPosition(-m_title->getGlobalBounds().width/2.0f, -m_title->getGlobalBounds().height-90);

    std::shared_ptr<BasicButton> bPlay(std::make_shared<BasicButton>
            (AdaptableBounds(sf::Vector2f(0, -10), sf::Vector2f(100, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER), tr("New")));
    bPlay->changeActiveState(Widget::ControlState::ACTIVE);

    std::shared_ptr<BasicButton> bLoad(std::make_shared<BasicButton>
            (AdaptableBounds(sf::Vector2f(0, 20), sf::Vector2f(100, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER), tr("Load")));

    std::shared_ptr<BasicButton> bOptions(std::make_shared<BasicButton>
            (AdaptableBounds(sf::Vector2f(0, 50), sf::Vector2f(100, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER), tr("Options")));

    std::shared_ptr<BasicButton> bExit(std::make_shared<BasicButton>
            (AdaptableBounds(sf::Vector2f(0, 80), sf::Vector2f(100, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER), tr("Quit")));

    bPlay->connect(CommandType::MOVE_DOWN, bLoad);
    bLoad->connect(CommandType::MOVE_UP, bPlay);
    bLoad->connect(CommandType::MOVE_DOWN, bOptions);
    bOptions->connect(CommandType::MOVE_UP, bLoad);
    bOptions->connect(CommandType::MOVE_DOWN, bExit);
    bExit->connect(CommandType::MOVE_UP, bOptions);

    add(bPlay);
    bPlay->connectClickEvent(std::bind(&newGameFunction, this));
    add(bLoad);
    bLoad->connectClickEvent(std::bind(&LoadFunction, this));
    add(bOptions);
    bOptions->connectClickEvent(std::bind(&OptionsFunction, this));
    add(bExit);
    bExit->connectClickEvent(std::bind(&exitFunction, this));

    //Event<EventPlayMusic>::send(EventPlayMusic("res/music/ZoneAlpha.ogg", MusicData(1, 1, true), 2, 2, 0));
}

MenuState::~MenuState()
{

}

void MenuState::enable()
{
    Event<EventInstantCenterOfViewChanged>::send(EventInstantCenterOfViewChanged(sf::Vector2f(0, 0)));

    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
        m->setClearColor(sf::Color::White);

    DrawableList::add(m_title, 1);

    for(auto & w : m_buttons)
    {
        DrawableList::add(w, 1);
        Controlable::add(w);
        Updatable::add(w);
    }
}

void MenuState::disable()
{
    DrawableList::del(m_title);

    for(auto & w : m_buttons)
    {
        DrawableList::del(w);
        Controlable::del(w);
        Updatable::del(w);
    }
}

void MenuState::add(std::shared_ptr<Widget> w)
{
    m_buttons.push_back(w);
}

void MenuState::newGameFunction()
{
    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
    {
        std::unique_ptr<State> s(std::make_unique<GameState>(m_machine));
        m->setNext(s);
    }
}

void MenuState::LoadFunction()
{
    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
    {
        std::unique_ptr<State> s(std::make_unique<LoadSaveState>(m_machine, true));
        m->setNext(s);
    }
}

void MenuState::OptionsFunction()
{
    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
    {
        std::unique_ptr<State> s(std::make_unique<OptionsState>(m_machine));
        m->addSubstate(s);
    }
}

void MenuState::exitFunction()
{
    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
        m->getWindow().close();
}

