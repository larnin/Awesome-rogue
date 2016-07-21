#include "mapteststate.h"
#include "Systemes/drawablelist.h"
#include "Map/Generator/generator.h"
#include "Machine/statemachine.h"
#include "Map/blocktype.h"
#include "Entities/Types/squaremob.h"
#include "Entities/Types/trackermob.h"
#include "Entities/Types/crosslasermob.h"
#include "Entities/Types/punchballmob.h"
#include "Entities/Types/circlemob.h"
#include "Events/event.h"
#include "Events/Datas/eventplayerchangeroom.h"
#include "Events/Datas/eventinstantcenterofviewchanged.h"
#include "Events/Datas/eventpreplayerchangeroom.h"
#include "Entities/entityfactory.h"
#include "Entities/entitytype.h"
#include "Entities/entitylist.h"
#include "Projectiles/ProjectileLauncher/parallelebulletlauncher.h"
#include "Events/eventgetter.h"
#include "Entities/Types/player.h"
#include "Projectiles/ProjectileLauncher/projectilelauncher.h"
#include "Map/map.h"
#include "Map/Render/worldrender.h"
#include "GUI/LifeBar/lifebar.h"
#include "GUI/gameinterface.h"
#include "GUI/minimap.h"
#include "Machine/listholder.h"
#include "Entities/populator.h"
#include "GUI/interactor.h"
#include "Map/room.h"

MapTestState::MapTestState(std::weak_ptr<StateMachine> machine)
    : State(machine)
{
    /*Patern p(sf::Vector2u(15, 5));
    p = p.transform(Rotation::ROT_180, true, true);
    r = std::make_shared<Room>(p, sf::Vector2i(2, 2));
    rr = std::make_shared<RoomRender>(r);
    DrawableList::add(rr, 2);*/

    listes = std::make_shared<ListHolder>();
    Updatable::add(listes);

    std::shared_ptr<StateMachine> mLock(m_machine.lock());
    sf::View v(mLock->getWindow().getView());
    v.zoom(1.0f);
    mLock->getWindow().setView(v);
    mLock->setClearColor(sf::Color::Black);

    Generator g;
    GenerationEnvironement env;
    env.paternsFileName = "res/paterns.json";
    map = std::make_shared<Map>(g.generate(env));
    const auto & r(map->room(0));
    r->uncover();

    if(mLock)
        mapRender = std::make_shared<WorldRender>(map, 0, mLock->getWindow().getSize());
    DrawableList::add(mapRender, 0);

    std::shared_ptr<Player> p(std::make_shared<Player>(Player(Location(r->getSize()/2u, r))));
    player = p;
    listes->entities.addEntity(p);
    Controlable::add(p);
    EventSimpleGetter<std::shared_ptr<Player>>::connect(std::bind(&MapTestState::getPlayer, this));

    projectilesLauncher = std::make_shared<ParalleleBulletLauncher>(p, 2, 0.15f);
    Controlable::add(projectilesLauncher);
    Updatable::add(projectilesLauncher);

    interface = std::make_shared<GameInterface>();
    DrawableList::add(interface, 6);
    minimap = std::make_shared<Minimap>(map);
    DrawableList::add(minimap, 5);

    lifeBar = std::make_shared<LifeBar>(p);
    DrawableList::add(lifeBar, 7);

    populator = std::make_shared<Populator>();
    Updatable::add(populator);

    interactor = std::make_shared<Interactor>(p);
    DrawableList::add(interactor, 5);
    Controlable::add(interactor);

    EntityFactory::create(E_BOSS1_PARTS, Location(r->getSize()/2u, r));

    Event<EventPrePlayerChangeRoom>::send(EventPrePlayerChangeRoom(p->getID()));
    Event<EventPlayerChangeRoom>::send(EventPlayerChangeRoom(p->getID()));
    Event<EventInstantCenterOfViewChanged>::send(EventInstantCenterOfViewChanged(p->getPos().toGlobalPos()));
}

MapTestState::~MapTestState()
{
    EventSimpleGetter<std::shared_ptr<Player>>::disconnect();
}

std::shared_ptr<Player> MapTestState::getPlayer()
{
    return player.lock();
}
