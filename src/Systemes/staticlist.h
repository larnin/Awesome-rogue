#ifndef STATICLIST
#define STATICLIST

#include <algorithm>
#include <vector>
#include <memory>

template <typename T>
class StaticList
{
public:
    static void add(std::weak_ptr<T> object)
    {
        std::shared_ptr<T> objectLock(object.lock());
        if(!objectLock)
            return;
        auto it(std::find_if(m_objects.begin(), m_objects.end(), [objectLock](std::weak_ptr<T> o){ return o.lock() == objectLock; }));
        if(it != m_objects.end())
            return;
        m_objects.push_back(object);
    }

    static void del(std::weak_ptr<T> object)
    {
        std::shared_ptr<T> objectLock(object.lock());
        auto it(std::find_if(m_objects.begin(), m_objects.end(), [objectLock](std::weak_ptr<T> o){ return o.lock() == objectLock; }));
        if(it == m_objects.end())
            return;
        std::swap(*it, m_objects.back());
        m_objects.pop_back();
    }

    static void clean()
    {
        std::vector<std::weak_ptr<T>> toDelete;
        for(auto & object : m_objects)
        {
            std::shared_ptr<T> objectLock(object.lock());
            if(!objectLock)
                toDelete.push_back(object);
        }
        for(auto & object : toDelete)
            del(object);
    }

protected:
    static std::vector<std::weak_ptr<T>> m_objects;
};

template <typename T>
std::vector<std::weak_ptr<T>> StaticList<T>::m_objects;
#endif // STATICLIST
