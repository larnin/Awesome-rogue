#ifndef MAPSTATE_H
#define MAPSTATE_H

#include "state.h"
#include <memory>

class GlobalMapRender;
class SimpleControler;

class MapState : public State
{
public:
    MapState(std::weak_ptr<StateMachine> machine, bool tp = false);
    MapState(MapState &&) = default;
    MapState & operator=(MapState &&) = default;
    virtual ~MapState() = default;

protected:
    virtual void enable() override;
    virtual void disable() override;

private:
    void onExitState();
    void onExecTp();

    bool m_tp;
    std::shared_ptr<GlobalMapRender> m_mapRender;
    std::shared_ptr<SimpleControler>m_controler;
};

#endif // MAPSTATE_H
