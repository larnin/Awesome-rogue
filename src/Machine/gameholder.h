#ifndef GAMEHOLDER_H
#define GAMEHOLDER_H

#include "Utilities/noncopiable.h"
#include "Entities/populator.h"
#include "Events/eventreceiver.h"
#include "Entities/playerinfos.h"
#include <memory>

class StateMachine;
class Player;
class Map;
class WorldRender;
class GameInterface;
class Minimap;
class LifeBar;
class ListHolder;
class ProjectileLauncher;
class Interactor;
class BossLifeBar;
class EventSetBossLifeBar;
class ItemsList;
class LightRender;

class GameHolder : private NonCopiable, public EventReceiver
{
public:
    GameHolder(std::weak_ptr<StateMachine> machine);
    GameHolder(GameHolder &&) = default;
    GameHolder & operator =(GameHolder &&) = default;
    virtual ~GameHolder();

    void enable();
    void disable();

private:
    std::shared_ptr<Player> getPlayer();
    std::shared_ptr<Map> getMap();
    void onBossLifeBarSet(EventSetBossLifeBar e);

    std::shared_ptr<Map> m_map;
    std::shared_ptr<WorldRender> m_mapRender;
    std::weak_ptr<Player> m_player;
    std::shared_ptr<GameInterface> m_interface;
    std::shared_ptr<Minimap> m_minimap;
    std::shared_ptr<LifeBar> m_lifeBar;
    std::shared_ptr<ListHolder> m_listes;
    std::shared_ptr<ProjectileLauncher> m_projectilesLauncher;
    Populator m_populator;
    std::shared_ptr<Interactor> m_interactor;
    std::shared_ptr<BossLifeBar> m_bossLifeBar;
    std::shared_ptr<ItemsList> m_items;
    PlayerInfos m_playerInfos;
    std::shared_ptr<LightRender> m_light;

    bool m_enabled;
};

#endif // GAMEHOLDER_H
