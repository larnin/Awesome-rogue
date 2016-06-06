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
#include "Entities/entityfactory.h"
#include "Entities/entitytype.h"
#include "Entities/entitylist.h"

MapTestState::MapTestState(std::weak_ptr<StateMachine> machine)
    : State(machine)
{
    /*Patern p(sf::Vector2u(15, 5));
    p = p.transform(Rotation::ROT_180, true, true);
    r = std::make_shared<Room>(p, sf::Vector2i(2, 2));
    rr = std::make_shared<RoomRender>(r);
    DrawableList::add(rr, 2);*/

    Generator g;
    std::shared_ptr<StateMachine> mLock(m_machine.lock());
    sf::View v(mLock->getWindow().getView());
    v.zoom(1.0f);
    mLock->getWindow().setView(v);
    mLock->setClearColor(sf::Color::Black);

    map = std::make_shared<Map>(g.generate(GenerationEnvironement()));
    map->room(0)->uncover();
    (*(map->room(0)))(sf::Vector2u(8, 8)).wallID = 64;
    (*(map->room(0)))(sf::Vector2u(8, 8)).boxCaracts = createBoxCaracts(Rotation::ROT_0, false, false, BoxType::FULL);
    (*(map->room(0)))(sf::Vector2u(9, 8)).wallID = 64;
    (*(map->room(0)))(sf::Vector2u(9, 8)).boxCaracts = createBoxCaracts(Rotation::ROT_0, false, false, BoxType::FULL);
    (*(map->room(0)))(sf::Vector2u(10, 8)).wallID = 64;
    (*(map->room(0)))(sf::Vector2u(10, 8)).boxCaracts = createBoxCaracts(Rotation::ROT_0, false, false, BoxType::FULL);
    (*(map->room(0)))(sf::Vector2u(8, 9)).wallID = 64;
    (*(map->room(0)))(sf::Vector2u(8, 9)).boxCaracts = createBoxCaracts(Rotation::ROT_0, false, false, BoxType::FULL);
    (*(map->room(0)))(sf::Vector2u(8, 10)).wallID = 64;
    (*(map->room(0)))(sf::Vector2u(8, 10)).boxCaracts = createBoxCaracts(Rotation::ROT_0, false, false, BoxType::FULL);
    if(mLock)
        mapRender = std::make_shared<WorldRender>(map, 0, mLock->getWindow().getSize());
    DrawableList::add(mapRender, 0);

    player = std::make_shared<Player>(Player(Location(sf::Vector2u(5, 5), map->room(0))));
    entities.addEntity(player);
    //DrawableList::add(player, 1);
    //Updatable::add(player);
    Controlable::add(player);

   interface = std::make_shared<GameInterface>();
   DrawableList::add(interface, 3);
   minimap = std::make_shared<Minimap>(map);
   DrawableList::add(minimap, 2);

   lifeBar = std::make_shared<LifeBar>(player);
   DrawableList::add(lifeBar, 4);

    /*for(unsigned int i(0) ; i < 10 ; i++)
        EntityFactory::create(EntityType::E_SQUARE_MOB, Location(sf::Vector2u(10, 10), map->room(0)));

    for(unsigned int i(0) ; i < 10 ; i++)
        EntityFactory::create(EntityType::E_CROSS_LASER_MOB, Location(sf::Vector2u(10, 10), map->room(0)));

    for(unsigned int i(0) ; i < 10 ; i++)
        EntityFactory::create(EntityType::E_TRACKER_MOB, Location(sf::Vector2u(10, 10), map->room(0)));*/

    for(unsigned int i(0) ; i < 1 ; i++)
        EntityFactory::create(EntityType::E_PUNCHBALL_MOB, Location(sf::Vector2u(10, 10), map->room(0)));

    /*for(unsigned int i(0) ; i < 10 ; i++)
        EntityFactory::create(EntityType::E_CIRCLE_MOB, Location(sf::Vector2u(10, 10), map->room(0)));

    for(unsigned int i(0) ; i < 10 ; i++)
        EntityFactory::create(EntityType::E_SMALL_CIRCLE_MOB, Location(sf::Vector2u(10, 10), map->room(0)));*/

    Event<EventPrePlayerChangeRoom>::send(EventPrePlayerChangeRoom(player->getID()));
    Event<EventPlayerChangeRoom>::send(EventPlayerChangeRoom(player->getID()));
}