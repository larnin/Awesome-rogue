#include "drawablelist.h"
#include <SFML/Graphics/Drawable.hpp>
#include <algorithm>

std::map<unsigned int, std::vector<std::weak_ptr<sf::Drawable>>> DrawableList::m_objects;
std::vector<DrawableList::ObjectModif> DrawableList::m_modification;

void DrawableList::add(std::weak_ptr<sf::Drawable> object, unsigned int height)
{
    m_modification.push_back(ObjectModif(object, height));
}

void DrawableList::del(std::weak_ptr<sf::Drawable> object)
{
    m_modification.push_back(ObjectModif(object));
}

void DrawableList::clean()
{
    for(auto & list : m_objects)
    {
        auto it(std::remove_if(list.second.begin(), list.second.end(), [](const auto & obj){return !obj.lock();}));
        list.second.erase(it, list.second.end());
        /*std::vector<std::weak_ptr<sf::Drawable>> toDelete;
        for(auto & object : list.second)
        {
            std::shared_ptr<sf::Drawable> objectLock(object.lock());
            if(!objectLock)
                toDelete.push_back(object);
        }
        for(auto & object : toDelete)
            delPrivate(object);*/
    }
}

void DrawableList::drawAll(sf::RenderTarget & target, sf::RenderStates states)
{
    applyModifications();

    for(const auto & list : m_objects)
        for(const auto & object : list.second)
        {
            const std::shared_ptr<sf::Drawable> objectLock(object.lock());
            if(objectLock)
                target.draw(*objectLock, states);
        }
    clean();
}

void DrawableList::addPrivate(std::weak_ptr<sf::Drawable> object, unsigned int height)
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

void DrawableList::delPrivate(std::weak_ptr<sf::Drawable> object)
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

void DrawableList::applyModifications()
{
    for(const auto & object : m_modification)
    {
        if(object.toAdd)
            addPrivate(object.object, object.height);
        else delPrivate(object.object);
    }
    m_modification.clear();
}
