#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <functional>
#include <memory>
#include <algorithm>

#include <cassert>

using namespace std::placeholders;

class EventReceiver;
template <typename T>
class Event;

struct EventRemover
{
    virtual void disconnect(EventReceiver* e) = 0;
    virtual ~EventRemover() = default;
};

template <typename T>
struct TypedEventRemover : public EventRemover
{
    virtual void disconnect(EventReceiver* e)
    {
        Event<T>::disconnect(e);
    }

    virtual ~TypedEventRemover() = default;
};

class EventRemoverList
{
    template <typename T>
    friend class Event;

public :
    static void disconnect(EventReceiver* e);

private:
    template <typename T>
    static void addRemover(const TypedEventRemover<T> & remover)
    {
        m_events.push_back(std::make_unique<TypedEventRemover<T>>(remover));
    }

    static std::vector<std::unique_ptr<EventRemover>> m_events;
};

template <typename T>
class Event
{
    friend class EventReceiver;
    friend class TypedEventRemover<T>;

public :
    static void send(T value)
    {
        for(unsigned int i(0) ; i < m_functions.size() ; i++)
        {
            auto & f(m_functions[i]);
            if(f.first != nullptr && f.second)
                f.second(value);
        }

        clean();
    }

private:
    Event() = delete;

    static void connect(EventReceiver* e, std::function<void(T)> funct)
    {
        assert(funct);
        m_functions.push_back(std::pair<EventReceiver*,std::function<void(T)>>(e, funct));
        if(!m_initialised)
        {
            EventRemoverList::addRemover<T>(TypedEventRemover<T>());
            m_initialised = true;
        }
    }

    static void disconnect(EventReceiver* e)
    {
        for(auto & f : m_functions)
            if(f.first == e)
                f.first = nullptr;
    }

    static void clean()
    {
        auto it = std::remove_if(m_functions.begin(), m_functions.end(), [](const auto & value){return value.first == nullptr;});
        m_functions.erase(it, m_functions.end());
    }

    static std::vector<std::pair<EventReceiver*,std::function<void(T)>>> m_functions;
    static bool m_initialised;
};

template <typename T>
std::vector<std::pair<EventReceiver*,std::function<void(T)>>> Event<T>::m_functions;
template <typename T>
bool Event<T>::m_initialised(false);



#endif // EVENT_H
