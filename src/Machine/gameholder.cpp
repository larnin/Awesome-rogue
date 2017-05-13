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
#include "GUI/LifeBar/bosslifebar.h"
#include "Entities/entityfactory.h"
#include "Events/Datas/eventsetbosslifebar.h"
#include "Events/Datas/eventinstantcenterofviewchanged.h"
#include "Events/Datas/eventinteraction.h"
#include "Events/Datas/eventitemloaded.h"
#include "Events/Datas/eventpreplayerchangeroom.h"
#include "Events/Datas/eventplayerchangeroom.h"
#include "Map/blocktype.h"
#include "Items/itemslist.h"
#include "File/serializer.h"
#include "Events/Datas/eventloadfinished.h"

#include <iostream>

GameHolder::GameHolder(std::weak_ptr<StateMachine> machine, const std::string & fileName)
    : m_enabled(false)
{
    initEvents();

    auto m(machine.lock());
    assert(m);

    m_map = std::make_shared<Map>();
    m_mapRender = std::make_shared<WorldRender>(m_map, 0, m->getWindow().getSize());

    m_listes = std::make_shared<ListHolder>();

    m_interface = std::make_shared<GameInterface>();
    m_minimap = std::make_shared<Minimap>(m_map);
    m_lifeBar = std::make_shared<LifeBar>();

    m_items = std::make_shared<ItemsList>();

    load(fileName);

    auto p(m_player.lock());
    if(!p)
        return;
    Event<EventPrePlayerChangeRoom>::send(EventPrePlayerChangeRoom(p->getID()));
    Event<EventPlayerChangeRoom>::send(EventPlayerChangeRoom(p->getID()));
}

GameHolder::GameHolder(std::weak_ptr<StateMachine> machine, const GenerationEnvironement & e)
    : m_enabled(false)
{
    initEvents();

    auto m(machine.lock());
    assert(m);

    Generator g;
    m_map = g.generate(e);
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

    m_items = std::make_shared<ItemsList>();
}

void GameHolder::initEvents()
{
    EventSimpleGetter<std::shared_ptr<Player>>::connect(std::bind(&GameHolder::getPlayer, this));
    EventSimpleGetter<std::shared_ptr<Map>>::connect(std::bind(&GameHolder::getMap, this));
    connect<EventSetBossLifeBar>(std::bind(&onBossLifeBarSet, this, _1));
    connect<EventItemLoaded<Player>>(std::bind(&onPlayerLoaded, this, _1));
    connect<EventItemLoaded<ItemsList>>(std::bind(&onItemListLoaded, this, _1));
    connect<EventLoadFinished>(std::bind(&onLoadFinish, this, _1));
}

GameHolder::~GameHolder()
{
     EventSimpleGetter<std::shared_ptr<Player>>::disconnect();
     EventSimpleGetter<std::shared_ptr<Map>>::disconnect();
}

void GameHolder::enable()
{
    m_enabled = true;

    DrawableList::add(m_mapRender, DrawableList::DrawHeight::MAP_BACK-1);
    m_mapRender->enable();

    std::shared_ptr<Player> p(m_player.lock());
    if(p)
    {
        Controlable::add(p);
        Event<EventInstantCenterOfViewChanged>::send(EventInstantCenterOfViewChanged(p->getPos().toGlobalPos()*float(BlockType::tileSize)));
    }

    Updatable::add(m_listes);
    m_listes->entities.enable();
    m_listes->projectiles.enable();
    m_listes->particules.enable();

    Updatable::add(m_projectilesLauncher);
    Controlable::add(m_projectilesLauncher);

    DrawableList::add(m_interface, DrawableList::DrawHeight::INTERFACE);
    DrawableList::add(m_minimap, DrawableList::DrawHeight::INTERFACE-1);
    DrawableList::add(m_lifeBar, DrawableList::DrawHeight::INTERFACE+1);

    m_populator.enable();

    DrawableList::add(m_interactor, DrawableList::DrawHeight::BLOCK_INTERACTION);
    Controlable::add(m_interactor);

    if(m_bossLifeBar)
    {
        DrawableList::add(m_bossLifeBar, DrawableList::DrawHeight::INTERFACE);
        Updatable::add(m_bossLifeBar);
    }

    DrawableList::add(m_items, DrawableList::DrawHeight::ITEM);
    Updatable::add(m_items);
}

void GameHolder::disable()
{
    m_enabled = false;

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

    if(m_bossLifeBar)
    {
        DrawableList::del(m_bossLifeBar);
        Updatable::del(m_bossLifeBar);
    }

    DrawableList::del(m_items);
    Updatable::del(m_items);
}

std::shared_ptr<Player> GameHolder::getPlayer()
{
    return m_player.lock();
}

std::shared_ptr<Map> GameHolder::getMap()
{
    return m_map;
}

void GameHolder::onBossLifeBarSet(EventSetBossLifeBar e)
{
    if(m_bossLifeBar && m_enabled)
    {
        DrawableList::del(m_bossLifeBar);
        Updatable::del(m_bossLifeBar);
    }
    m_bossLifeBar = e.lifeBar;
    if(m_bossLifeBar && m_enabled)
    {

        DrawableList::add(m_bossLifeBar, DrawableList::DrawHeight::INTERFACE);
        Updatable::add(m_bossLifeBar);
    }
}

void GameHolder::onPlayerLoaded(EventItemLoaded<Player> e)
{
    m_player = e.item;
    m_lifeBar->setEntity(e.item);
    m_projectilesLauncher = std::make_shared<ParalleleBulletLauncher>(e.item, 2, 0.15f);
    m_interactor = std::make_shared<Interactor>(e.item);
}

void GameHolder::onItemListLoaded(EventItemLoaded<ItemsList> e)
{
    m_items = e.item;
}

void GameHolder::onLoadFinish(EventLoadFinished)
{

}
