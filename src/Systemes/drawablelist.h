#ifndef DRAWABLE_LIST_H
#define DRAWABLE_LIST_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <map>
#include <vector>
#include <memory>

class DrawableList
{
public:
    enum DrawHeight
    {
        BACK = 0,
        MAP_BACK = 100,
        PROJECTILE = 200,
        ITEM = 300,
        ENTITY = 400,
        PARTICLE = 500,
        MAP_TOP = 600,
        LIGHT = 700,
        BLOCK_INTERACTION = 800,
        INTERFACE = 900,
        MENU = 1000
    };

    static void add(std::weak_ptr<sf::Drawable> object, unsigned int height);
    static void del(std::weak_ptr<sf::Drawable> object);
    static void clean();
    static void drawAll(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates());

private:
    struct ObjectModif
    {
        ObjectModif(std::weak_ptr<sf::Drawable> _object, unsigned int _height)
            : object(_object), height(_height), toAdd(true) {}
        ObjectModif(std::weak_ptr<sf::Drawable> _object)
            : object(_object), height(0), toAdd(false) {}

        std::weak_ptr<sf::Drawable> object;
        unsigned int height;
        bool toAdd;
    };

    DrawableList() = delete;

    static void addPrivate(std::weak_ptr<sf::Drawable> object, unsigned int height);
    static void delPrivate(std::weak_ptr<sf::Drawable> object);
    static void applyModifications();

    static std::map<unsigned int, std::vector<std::weak_ptr<sf::Drawable>>> m_objects;
    static std::vector<ObjectModif> m_modification;
};

#endif // DRAWABLE_LIST_H
