#ifndef EVENTGETTER_H
#define EVENTGETTER_H

#include <cassert>
#include <functional>
#include <algorithm>
#include <stdexcept>

using namespace std::placeholders;

template <typename T, typename U>
class EventGetter
{
public:
    static T get(U value)
    {
        assert(m_function);

        return m_function(value);
    }

    static void connect(std::function<T(U)> f)
    {
        assert(!m_function);
        m_function = f;
    }

    static void disconnect()
    {
        assert(m_function);
        m_function = std::function<T(U)>();
    }

private:
    EventGetter() = delete;

    static std::function<T(U)> m_function;
};

template <typename T, typename U>
std::function<T(U)> EventGetter<T,U>::m_function;


template <typename T>
class EventSimpleGetter
{
public:
    static T get()
    {
        if(!m_function)
            throw std::runtime_error("No function set !");
        return m_function();
    }

    static bool connect(std::function<T()> f)
    {
        if(m_function)
            return false;
        m_function = f;
        return true;
    }

    static bool disconnect()
    {
        if(!m_function)
            return false;
        m_function = std::function<T()>();
        return true;
    }

private:
    EventGetter() = delete;

    static std::function<T()> m_function;
};

template <typename T>
std::function<T()> EventSimpleGetter<T>::m_function;

#endif // EVENTGETTER_H
