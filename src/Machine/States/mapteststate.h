#ifndef MAPTESTSTATE_H
#define MAPTESTSTATE_H

#include <memory>
#include <vector>
#include "state.h"

class Player;
class Map;
class WorldRender;
class GameInterface;
class Minimap;
class LifeBar;
class ListHolder;
class ProjectileLauncher;
class Populator;
class Interactor;

class MapTestState : public State
{
public:
    MapTestState(std::weak_ptr<StateMachine> machine);
    MapTestState(MapTestState&&) = default;
    MapTestState & operator= (MapTestState &&) = default;
    virtual ~MapTestState();

private:
    std::shared_ptr<Player> getPlayer();

    std::shared_ptr<Map> map;
    std::shared_ptr<WorldRender> mapRender;
    std::weak_ptr<Player> player;
    std::shared_ptr<GameInterface> interface;
    std::shared_ptr<Minimap> minimap;
    std::shared_ptr<LifeBar> lifeBar;
    std::shared_ptr<ListHolder> listes;
    std::shared_ptr<ProjectileLauncher> projectilesLauncher;
    std::shared_ptr<Populator> populator;
    std::shared_ptr<Interactor> interactor;
};

#endif // MAPTESTSTATE_H
