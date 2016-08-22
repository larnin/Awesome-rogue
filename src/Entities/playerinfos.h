#ifndef PLAYERINFOS_H
#define PLAYERINFOS_H

#include "Utilities/noncopiable.h"
#include "Events/eventreceiver.h"

class EventPickItem;

class PlayerInfos : public EventReceiver, private NonCopiable
{
public:
    PlayerInfos();
    PlayerInfos(PlayerInfos &&) = default;
    PlayerInfos & operator =(PlayerInfos &&) = default;
    virtual ~PlayerInfos() = default;

private:
    void onPickItem(EventPickItem e);

    unsigned int m_experience;

};

#endif // PLAYERINFOS_H
