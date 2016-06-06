#include <algorithm>
#include <SFML/Graphics/Drawable.hpp>
#include "drawablelist.h"

std::map<unsigned int, std::vector<std::weak_ptr<sf::Drawable>>> DrawableList::m_objects;

void DrawableList::add(std::weak_ptr<sf::Drawable> object, unsigned int height)
{
    std::shared_ptr<sf::Drawable> objectLock(object.lock());
    if(!objectLock)
        return;
    for(const auto & list : m_objects)
    {
        if(std::find_if(list.second.begin(), list.second.end()
                        , [objectLock](std::weak_ptr<sf::Drawable> o){return o.lock() == objectLock;}) != list.second.end())
            return;
    }
    auto id(m_objects.find(height));
    if(id == m_objects.end())
        m_objects.emplace(height, std::vector<std::weak_ptr<sf::Drawable>>(1, object));
    else id->second.push_back(object);
}

void DrawableList::del(std::weak_ptr<sf::Drawable> object)
{
    std::shared_ptr<sf::Drawable> objectLock(object.lock());
    for(auto & list : m_objects)
    {
        auto it(std::find_if(list.second.begin(), list.second.end(), [objectLock](std::weak_ptr<sf::Drawable> o){return o.lock() == objectLock;}));
        if(it == list.second.end())
            continue;
        //if(list.second.size() <= 1)
        //    m_objects.erase(list.first);
        else
        {
            std::swap(*it, list.second.back());
            list.second.pop_back();
        }
        break;
    }
}

void DrawableList::clean()
{
    for(auto & list : m_objects)
    {
        std::vector<std::weak_ptr<sf::Drawable>> toDelete;
        for(auto & object : list.second)
        {
            std::shared_ptr<sf::Drawable> objectLock(object.lock());
            if(!objectLock)
                toDelete.push_back(object);
        }
        for(auto & object : toDelete)
            del(object);
    }
}

void DrawableList::drawAll(sf::RenderTarget & target, sf::RenderStates states)
{
    for(const auto & list : m_objects)
        for(const auto & object : list.second)
        {
            const std::shared_ptr<sf::Drawable> objectLock(object.lock());
            if(objectLock)
                target.draw(*objectLock, states);
        }
    clean();
}
