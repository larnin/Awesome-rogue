#ifndef DRAWABLE_LIST_H
#define DRAWABLE_LIST_H

#include <map>
#include <vector>
#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class DrawableList
{
public:
    enum DrawHeight
    {
        BACK = 0,
        GAME_MAP = 40,
        GAME_ENTITY = 50,
        GAME_INTERFACE = 60,
        POPUP_WIDGET = 100
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
