#include "mapstate.h"
#include "GUI/globalmaprender.h"
#include "Events/eventgetter.h"
#include "Systemes/drawablelist.h"
#include "Machine/statemachine.h"
#include "GUI/simplecontroler.h"
#include "Events/event.h"
#include "Events/Datas/eventpreplayerchangeroom.h"
#include "Events/Datas/eventplayerchangeroom.h"
#include "Entities/Types/player.h"

MapState::MapState(std::weak_ptr<StateMachine> machine, bool tp)
    : State(machine)
    , m_tp(tp)
{
    auto map(EventSimpleGetter<std::shared_ptr<Map>>::get());
    m_mapRender = std::make_shared<GlobalMapRender>(map, tp);

    std::shared_ptr<StateMachine> m(machine.lock());
    if(m)
        m->setClearColor(sf::Color::Black);

    m_controler = std::make_shared<SimpleControler>();
    m_controler->set(KEY_CANCEL, std::bind(&onExitState, this));
    if(tp)
        m_controler->set(KEY_VALID, std::bind(&onExecTp, this));
}

void MapState::enable()
{
    DrawableList::add(m_mapRender, DrawableList::DrawHeight::BACK);
    Controlable::add(m_mapRender);
    Updatable::add(m_mapRender);

    Controlable::add(m_controler);
}

void MapState::disable()
{
    DrawableList::del(m_mapRender);
    Controlable::del(m_mapRender);
    Updatable::add(m_mapRender);

    Controlable::del(m_controler);
}

void MapState::onExitState()
{
    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
        m->delSubstate();
}

void MapState::onExecTp()
{
    Location pos(m_mapRender->getCurrentPortal());
    auto p(EventSimpleGetter<std::shared_ptr<Player>>::get());
    if(p)
    {
        p->jumpTo(pos);
        Event<EventPrePlayerChangeRoom>::send(EventPrePlayerChangeRoom(p->getID()));
        Event<EventPlayerChangeRoom>::send(EventPlayerChangeRoom(p->getID()));
    }

    std::shared_ptr<StateMachine> m(m_machine.lock());
    if(m)
        m->delSubstate();
}
