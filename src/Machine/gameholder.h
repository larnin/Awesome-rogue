#ifndef GAMEHOLDER_H
#define GAMEHOLDER_H

#include "Utilities/noncopiable.h"
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
class Populator;
class Interactor;

class GameHolder : private NonCopiable
{
public:
    GameHolder(std::weak_ptr<StateMachine> machine);
    GameHolder(GameHolder &&) = default;
    GameHolder & operator =(GameHolder &&) = default;
    ~GameHolder() = default;

    void enable();
    void disable();

private:
    std::shared_ptr<Player> getPlayer();

    std::shared_ptr<Map> m_map;
    std::shared_ptr<WorldRender> m_mapRender;
    std::weak_ptr<Player> m_player;
    std::shared_ptr<GameInterface> m_interface;
    std::shared_ptr<Minimap> m_minimap;
    std::shared_ptr<LifeBar> m_lifeBar;
    std::shared_ptr<ListHolder> m_listes;
    std::shared_ptr<ProjectileLauncher> m_projectilesLauncher;
    std::shared_ptr<Populator> m_populator;
    std::shared_ptr<Interactor> m_interactor;
};

#endif // GAMEHOLDER_H
