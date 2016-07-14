#ifndef MAPTESTSTATE_H
#define MAPTESTSTATE_H

#include <memory>
#include <vector>
#include "state.h"
#include "GUI/gameinterface.h"
#include "Map/map.h"
#include "Map/Render/worldrender.h"
#include "Entities/Types/player.h"
#include "Entities/Types/entity.h"
#include "GUI/minimap.h"
#include "GUI/LifeBar/lifebar.h"
#include "Machine/listholder.h"
#include "Projectiles/ProjectileLauncher/projectilelauncher.h"
#include "Entities/populator.h"
#include "GUI/interactor.h"

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
