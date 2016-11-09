#ifndef PLAYERINFOS_H
#define PLAYERINFOS_H

#include "Utilities/noncopiable.h"
#include "Events/eventreceiver.h"
#include "File/serializable.h"

class EventPickItem;

class PlayerInfos : public EventReceiver, public Serializable, private NonCopiable
{
public:
    PlayerInfos();
    PlayerInfos(const json & j);
    PlayerInfos(PlayerInfos &&) = default;
    PlayerInfos & operator =(PlayerInfos &&) = default;
    virtual ~PlayerInfos() = default;

protected:
    virtual json serialize() const;

private:
    void onPickItem(EventPickItem e);

    unsigned int m_experience;

};

#endif // PLAYERINFOS_H
