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
#include "GUI/lifebar.h"

class MapTestState : public State
{
public:
    MapTestState(std::weak_ptr<StateMachine> machine);
    MapTestState(MapTestState&&) = default;
    MapTestState & operator= (MapTestState &&) = default;
    virtual ~MapTestState() = default;

private:
    std::shared_ptr<Map> map;
    std::shared_ptr<WorldRender> mapRender;
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Entity>> mobs;
    std::shared_ptr<GameInterface> interface;
    std::shared_ptr<Minimap> minimap;
    std::shared_ptr<LifeBar> lifeBar;
};

#endif // MAPTESTSTATE_H
