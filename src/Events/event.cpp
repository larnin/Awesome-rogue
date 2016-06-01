#include "event.h"

std::vector<std::unique_ptr<EventRemover>> EventRemoverList::m_events;

void EventRemoverList::disconnect(EventReceiver* e)
{
    for(const auto & f : m_events)
        f->disconnect(e);
}
