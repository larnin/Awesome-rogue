#ifndef ITEMSLIST_H
#define ITEMSLIST_H

#include "Systemes/updatable.h"
#include "Utilities/noncopiable.h"
#include "Events/eventreceiver.h"
#include "itemtype.h"
#include "Utilities/ressource.h"
#include "File/serializable.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <vector>
#include <map>

class Location;
class EventDropItem;

class ItemsList : public Updatable, public sf::Drawable, public EventReceiver, public Serializable, private NonCopiable
{
public:
    ItemsList();
    ItemsList(const json & j);
    ItemsList(ItemsList &&) = default;
    ItemsList & operator=(ItemsList &&) = default;
    virtual ~ItemsList() = default;

    void addItem(ItemType type, const Location & pos, const sf::Vector2f & originalSpeed);

    virtual void update(const sf::Time & elapsedTime) override;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;

protected:
    virtual json serialize() const override;

private:
    struct ItemData
    {
        ItemData(ItemType _type, const sf::Vector2f & _pos, const sf::Vector2f & _speed)
            : pos(_pos), speed(_speed), type(_type) {}

        sf::Vector2f pos;
        sf::Vector2f speed;
        ItemType type;
    };

    void onItemDrop(EventDropItem e);
    void addItem(unsigned int roomID, const ItemData & item);

    std::map<unsigned int, std::vector<ItemData>> m_items;
    Texture m_texture;
};

#endif // ITEMSLIST_H
