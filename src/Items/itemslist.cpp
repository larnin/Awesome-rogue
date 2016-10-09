#include "itemslist.h"
#include "Map/room.h"
#include "Events/eventgetter.h"
#include "Entities/Types/player.h"
#include "Utilities/vect2convert.h"
#include "Collisions/collisions.h"
#include "Events/event.h"
#include "Events/Datas/eventpickitem.h"
#include "Map/location.h"
#include "Map/blocktype.h"
#include "Utilities/quadrender.h"
#include "Events/Datas/eventdropitem.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

ItemsList::ItemsList()
    : m_texture("res/img/items.png")
{
    connect<EventDropItem>(std::bind(&onItemDrop, this, _1));
}

void ItemsList::addItem(ItemType type, const Location & pos, const sf::Vector2f & originalSpeed)
{
    std::shared_ptr<Room> r(pos.getRoom().lock());
    if(!r)
        return;
    unsigned int rID(r->getID());

    if(m_items.find(rID) == m_items.end())
        m_items.emplace(rID, std::vector<ItemData>());

    m_items.find(rID)->second.push_back(ItemData(type, pos.getPos(), originalSpeed));
}

void ItemsList::update(const sf::Time & elapsedTime)
{
    const float limitMultiplier(1.5f);
    const float limitReductor(1.0f);
    const float attractionForce(5.0f);
    const float attractionDistance(5.0f);

    float time(elapsedTime.asSeconds());

    std::shared_ptr<Player> p(EventSimpleGetter<std::shared_ptr<Player>>::get());
    if(!p)
        return;
    sf::Vector2f pPos(p->getPos().getPos());

    std::shared_ptr<Room> r(p->getPos().getRoom().lock());
    if(!r)
        return;

    auto it(m_items.find(r->getID()));
    if(it == m_items.end())
        return;
    std::vector<ItemData> & items(it->second);

    for(ItemData & i : items)
    {
        float n(norm(i.speed)*(1-(limitMultiplier*time))-(limitReductor*time));
        if(n < 0)
            n = 0;
        i.speed = toVect(n, angle(i.speed));

        sf::Vector2f speed(i.speed*time);
        if(isExperienceType(i.type) && norm(i.pos-pPos) < attractionDistance)
        {
            float attractForce(attractionForce*(1-norm(pPos-i.pos)/attractionDistance));
            speed += time*attractForce*normalise(pPos-i.pos);
        }

        sf::Vector2f residualSpeed(speed);
        for(unsigned int a(0) ; a < 2 ; a++)
        {
            Collisions::InteractResult result(Collisions::interact(itemBox(i.type), i.pos, residualSpeed, r));
            if(!result.collision)
            {
                i.pos += speed;
                break;
            }
            else
            {
                sf::Vector2f localMove(result.endPos-i.pos);
                i.pos += localMove;
                float speedMultiplier(std::cos(result.surfaceAngle - angle(residualSpeed)));
                residualSpeed -= localMove;
                residualSpeed = toVect(norm(residualSpeed)*speedMultiplier, result.surfaceAngle);
                i.speed = toVect(norm(i.speed)*speedMultiplier, result.surfaceAngle);
            }
        }
    }

    auto itr(std::remove_if(items.begin(), items.end(), [pPos](const auto & i)
    {
        if(norm(i.pos-pPos) <= itemPickRadius(i.type))
        {
            Event<EventPickItem>::send(EventPickItem(i.type));
            return true;
        }
        return false;
    }));
    items.erase(itr, items.end());
}

void ItemsList::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    std::shared_ptr<Player> p(EventSimpleGetter<std::shared_ptr<Player>>::get());
    if(!p)
        return;

    std::shared_ptr<Room> r(p->getPos().getRoom().lock());
    if(!r)
        return;

    auto it(m_items.find(r->getID()));
    if(it == m_items.end())
        return;

    sf::VertexArray array(sf::Quads, it->second.size()*4);

    for(unsigned int i(0) ; i < it->second.size() ; i++)
    {
        const ItemData & item(it->second[i]);
        sf::FloatRect rect(itemTextureRect(item.type));
        sf::Vector2f pos((sf::Vector2f(r->getPos())+item.pos)*float(BlockType::tileSize));
        drawQuad(&array[i*4], sf::FloatRect(pos.x-rect.width/2, pos.y-rect.height/2, rect.width, rect.height), rect);
    }
    target.draw(array, sf::RenderStates(m_texture()));
}

void ItemsList::onItemDrop(EventDropItem e)
{
    addItem(e.type, e.pos, e.speed);
}
