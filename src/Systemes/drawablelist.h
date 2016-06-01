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
    DrawableList() = delete;
    static std::map<unsigned int, std::vector<std::weak_ptr<sf::Drawable>>> m_objects;
};

#endif // DRAWABLE_LIST_H
