#include "playerinfos.h"
#include "Events/Datas/eventpickitem.h"
#include "Events/Datas/eventexperiencechanged.h"

PlayerInfos::PlayerInfos()
    : Serializable(SERIALIZE_PLAYERINFOS)
    , m_experience(0)
{
    connect<EventPickItem>(std::bind(&onPickItem, this, _1));
}

PlayerInfos::PlayerInfos(const json & j)
    : PlayerInfos()
{
    m_experience = j["experience"];
}

void PlayerInfos::onPickItem(EventPickItem e)
{
    if(isExperienceType(e.type))
    {
        m_experience += experienceValue(e.type);
        Event<EventExperienceChanged>::send(EventExperienceChanged(m_experience));
    }
}

json PlayerInfos::serialize() const
{
    json j;
    j["experience"] = m_experience;
    return j;
}
