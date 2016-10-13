#include "pausestate.h"
#include "Machine/statemachine.h"
#include "Events/event.h"
#include "Events/Datas/eventinstantcenterofviewchanged.h"
#include "GUI/Widgets/Buttons/basicbutton.h"
#include "Systemes/drawablelist.h"
#include "menustate.h"
#include "optionsstate.h"
#include "mapstate.h"
#include "Utilities/tr.h"

PauseState::PauseState(std::weak_ptr<StateMachine> machine)
    : State(machine)
    , m_titleTexture("res/img/pause.png")
{
    m_title = std::make_shared<sf::Sprite>(*m_titleTexture);
    m_title->setPosition(-m_title->getGlobalBounds().width/2.0f, -m_title->getGlobalBounds().height-90);

    std::shared_ptr<BasicButton> bContinue(std::make_shared<BasicButton>
            (AdaptableBounds(sf::Vector2f(0, -10), sf::Vector2f(100, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER), tr("Continue")));
    bContinue->changeActiveState(Widget::ControlState::ACTIVE);

    std::shared_ptr<BasicButton> bMap(std::make_shared<BasicButton>
            (AdaptableBounds(sf::Vector2f(0, 20), sf::Vector2f(100, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER), tr("Map")));

    std::shared_ptr<BasicButton> bInv(std::make_shared<BasicButton>
            (AdaptableBounds(sf::Vector2f(0, 50), sf::Vector2f(100, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER), tr("Inventary")));

    std::shared_ptr<BasicButton> bOptions(std::make_shared<BasicButton>
            (AdaptableBounds(sf::Vector2f(0, 80), sf::Vector2f(100, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER), tr("Options")));

    std::shared_ptr<BasicButton> bQuit(std::make_shared<BasicButton>
            (AdaptableBounds(sf::Vector2f(0, 110), sf::Vector2f(100, 25), Margin(1), VAlign::V_CENTER, HAlign::H_CENTER), tr("Quit")));


    bContinue->connect(CommandType::MOVE_DOWN, bMap);
    bMap->connect(CommandType::MOVE_UP, bContinue);
    bMap->connect(CommandType::MOVE_DOWN, bInv);
    bInv->connect(CommandType::MOVE_UP, bMap);
    bInv->connect(CommandType::MOVE_DOWN, bOptions);
    bOptions->connect(CommandType::MOVE_UP, bInv);
    bOptions->connect(CommandType::MOVE_DOWN, bQuit);
    bQuit->connect(CommandType::MOVE_UP, bOptions);

    bContinue->connectClickEvent(std::bind(&continueFunction, this));
    bMap->connectClickEvent(std::bind(&mapFunction, this));
    bInv->connectClickEvent(std::bind(&inventaryFunction, this));
    bOptions->connectClickEvent(std::bind(&optionsFunction, this));
    bQuit->connectClickEvent(std::bind(&quitFunction, this));

    m_buttons.push_back(bContinue);
    m_buttons.push_back(bMap);
    m_buttons.push_back(bInv);
    m_buttons.push_back(bOptions);
    m_buttons.push_back(bQuit);
}

void PauseState::enable()
{
    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
        m->setClearColor(sf::Color::White);

    Event<EventInstantCenterOfViewChanged>::send(EventInstantCenterOfViewChanged(sf::Vector2f(0, 0)));

    DrawableList::add(m_title, 1);

    for(auto & w : m_buttons)
    {
        DrawableList::add(w, 1);
        Controlable::add(w);
        Updatable::add(w);
    }
}

void PauseState::disable()
{
    DrawableList::del(m_title);

    for(auto & w : m_buttons)
    {
        DrawableList::del(w);
        Controlable::del(w);
        Updatable::del(w);
    }
}

void PauseState::continueFunction()
{
    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
        m->delSubstate();
}

void PauseState::mapFunction()
{
    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
    {
        std::unique_ptr<State> s(std::make_unique<MapState>(m_machine));
        m->addSubstate(s);
    }
}

void PauseState::inventaryFunction()
{

}

void PauseState::optionsFunction()
{
    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
    {
        std::unique_ptr<State> s(std::make_unique<OptionsState>(m_machine));
        m->addSubstate(s);
    }
}

void PauseState::quitFunction()
{
    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
    {
        std::unique_ptr<State> s(std::make_unique<MenuState>(m_machine));
        m->setNext(s);
        m->delSubstate();
    }
}
