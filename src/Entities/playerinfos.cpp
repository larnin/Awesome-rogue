#include "playerinfos.h"
#include "Events/Datas/eventpickitem.h"
#include "Events/Datas/eventexperiencechanged.h"

PlayerInfos::PlayerInfos()
    : m_experience(0)
{
    connect<EventPickItem>(std::bind(&onPickItem, this, _1));
}

void PlayerInfos::onPickItem(EventPickItem e)
{
    if(isExperienceType(e.type))
    {
        m_experience += experienceValue(e.type);
        Event<EventExperienceChanged>::send(EventExperienceChanged(m_experience));
    }
}
