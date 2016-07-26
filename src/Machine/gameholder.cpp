#include "gameholder.h"
#include "Events/eventgetter.h"
#include "Entities/Types/player.h"
#include "Map/map.h"
#include "Map/Render/worldrender.h"
#include "GUI/gameinterface.h"
#include "GUI/minimap.h"
#include "GUI/LifeBar/lifebar.h"
#include "listholder.h"
#include "Projectiles/ProjectileLauncher/projectilelauncher.h"
#include "GUI/interactor.h"
#include "Map/Generator/generator.h"
#include "Machine/statemachine.h"
#include "Systemes/drawablelist.h"
#include "Map/room.h"
#include "Projectiles/ProjectileLauncher/parallelebulletlauncher.h"

#include "Entities/entityfactory.h"

GameHolder::GameHolder(std::weak_ptr<StateMachine> machine)
{
    auto m(machine.lock());
    assert(m);

    EventSimpleGetter<std::shared_ptr<Player>>::connect(std::bind(&GameHolder::getPlayer, this));

    Generator g;
    GenerationEnvironement e;
    e.paternsFileName = "res/paterns.json";
    e.posBoss[0] = 0.01;
    m_map = std::make_shared<Map>(g.generate(e));
    auto r(m_map->room(0));
    r->uncover();

    m_mapRender = std::make_shared<WorldRender>(m_map, 0, m->getWindow().getSize());

    m_listes = std::make_shared<ListHolder>();
    std::shared_ptr<Player> p(std::make_shared<Player>(Player(Location(r->getSize()/2u, r))));
    m_player = p;
    m_listes->entities.addEntity(p);

    m_projectilesLauncher = std::make_shared<ParalleleBulletLauncher>(p, 2, 0.15f);

    m_interface = std::make_shared<GameInterface>();
    m_minimap = std::make_shared<Minimap>(m_map);
    m_lifeBar = std::make_shared<LifeBar>(p);

    m_interactor = std::make_shared<Interactor>(p);
}

void GameHolder::enable()
{
    DrawableList::add(m_mapRender, 0);
    m_mapRender->enable();

    std::shared_ptr<Player> p(m_player.lock());
    if(p)
        Controlable::add(p);

    Updatable::add(m_listes);
    m_listes->entities.enable();
    m_listes->projectiles.enable();
    m_listes->particules.enable();

    Updatable::add(m_projectilesLauncher);
    Controlable::add(m_projectilesLauncher);

    DrawableList::add(m_interface, 6);
    DrawableList::add(m_minimap, 5);
    DrawableList::add(m_lifeBar, 7);

    m_populator.enable();

    DrawableList::add(m_interactor, 4);
    Controlable::add(m_interactor);
}

void GameHolder::disable()
{
    DrawableList::del(m_mapRender);
    m_mapRender->disable();

    std::shared_ptr<Player> p(m_player.lock());
    if(p)
        Controlable::del(p);

    Updatable::del(m_listes);
    m_listes->entities.disable();
    m_listes->projectiles.disable();
    m_listes->particules.disable();

    Updatable::del(m_projectilesLauncher);
    Controlable::del(m_projectilesLauncher);

    DrawableList::del(m_interface);
    DrawableList::del(m_minimap);
    DrawableList::del(m_lifeBar);

    m_populator.disable();

    DrawableList::del(m_interactor);
    Controlable::del(m_interactor);
}

std::shared_ptr<Player> GameHolder::getPlayer()
{
    return m_player.lock();
}
