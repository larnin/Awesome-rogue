#ifndef PARTICULELIST_H
#define PARTICULELIST_H

#include <vector>
#include "Events/eventreceiver.h"
#include "Events/Datas/eventparticulecreated.h"
#include "Events/Datas/eventpreplayerchangeroom.h"
#include "Utilities/noncopiable.h"

class ParticuleList: public EventReceiver , private NonCopiable
{
public:
    ParticuleList();
    ParticuleList(ParticuleList &&) = default;
    ParticuleList & operator =(ParticuleList &&) = default;
    virtual ~ParticuleList();

    void addParticule(std::shared_ptr<Particule> p);
    void removeParticule(std::shared_ptr<Particule> p);

    const std::vector<std::shared_ptr<Particule>> particules() const;

    void clean();
    void clear();

private:
    void onPlayerChangeRoom(EventPrePlayerChangeRoom e);
    void onParticuleCreated(EventParticuleCreated e);

    unsigned int m_playerRoom;

    std::vector<std::shared_ptr<Particule>> m_particules;

    static bool m_instanced;
};

#endif // PARTICULELIST_H
