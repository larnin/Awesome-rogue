#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <functional>
#include "event.h"

class EventReceiver
{
public:
    EventReceiver() = default;
    EventReceiver(const EventReceiver & e) = delete;
    EventReceiver & operator= (const EventReceiver & e) = delete;
    EventReceiver(EventReceiver &&) = default;
    EventReceiver & operator =(EventReceiver && e) = default;

    virtual ~EventReceiver()
    {
        disconnect();
    }

protected:
    template <typename T>
    void connect(std::function<void(T)> funct)
    {
        Event<T>::connect(this, funct);
    }

    inline void disconnect()
    {
        EventRemoverList::disconnect(this);
    }

};

#endif // EVENTRECEIVER_H
