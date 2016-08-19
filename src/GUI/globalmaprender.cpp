#include "globalmaprender.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Map/map.h"
#include "Map/room.h"
#include "Utilities/quadrender.h"
#include "Events/eventgetter.h"
#include "Entities/Types/player.h"
#include "Events/event.h"
#include "Events/Datas/eventinstantcenterofviewchanged.h"
#include "Events/Datas/eventcenterofviewchanged.h"
#include "Controles/commandsvalue.h"
#include "Utilities/vect2convert.h"

const sf::Color wallsColor(47, 52, 60);
const sf::Color groundColor(62, 74, 77);
const sf::Color doorColor(80, 94, 98);

const sf::FloatRect boss(0, 0, 11, 11);
const sf::FloatRect portal(12, 0, 7, 7);
const sf::FloatRect chest(20, 0, 7, 5);
const sf::FloatRect save(28, 0, 7, 6);
const sf::FloatRect door(36, 0, 5, 4);
const sf::FloatRect player(42, 0, 7, 7);

GlobalMapRender::GlobalMapRender(std::weak_ptr<Map> map, bool canTp)
    : Controlable(ControlState::ACTIVE)
    , m_mapRender(sf::Quads)
    , m_iconRender(sf::Quads)
    , m_iconTexture("res/img/mapIcons.png")
    , m_canTp(canTp)
    , m_portalID(0)
    , m_tile(4)
    , m_map(map)
{
    std::shared_ptr<Map> m(map.lock());
    if(m)
    {
        generateRender(m);
        createIcons(m);
        generateIcons();
        if(canTp)
            movePortailIDToCurrent();
    }

    if(canTp)
        moveCameraToCurrentPortal(true);
    else
    {
        auto p(EventSimpleGetter<std::shared_ptr<Player>>::get());
        if(p)
        {
            m_pos = p->getPos().toGlobalPos()*m_tile;
            Event<EventInstantCenterOfViewChanged>::send(EventInstantCenterOfViewChanged(m_pos));
        }
    }

}

void GlobalMapRender::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    target.draw(m_mapRender);
    target.draw(m_iconRender, sf::RenderStates(m_iconTexture()));

    if(m_canTp && !m_portals.empty())
        drawPortalCursor(target);
}

void GlobalMapRender::drawPortalCursor(sf::RenderTarget & target) const
{
    sf::Vector2f pos(m_portals[m_portalID].toGlobalPos()*m_tile);

    sf::Sprite s(*m_iconTexture, sf::IntRect(portal));
    s.setOrigin(portal.width/2, portal.height/2);
    s.move(pos);

    float scale(2+std::sin(1.5*m_time)*0.6);
    s.scale(scale, scale);
    s.rotate(m_time*80);
    target.draw(s);
}

void GlobalMapRender::control(CommandsValue & v)
{
    sf::Vector2f dir(v.getValue(MOVE_RIGHT)-v.getValue(MOVE_LEFT), v.getValue(MOVE_DOWN)-v.getValue(MOVE_UP));
    if(norm(dir) > 1.0f)
        dir = normalise(dir);
    m_moveDir = dir;
    if(!m_canTp)
    {
        if(v.isPressEvent(KEY_ACTION))
        {
            v.valide(KEY_ACTION);
            onChangeZoom();
        }
    }
    else
    {
        if(v.isPressEvent(KEY_LEFT_PAGE))
        {
            v.valide(KEY_LEFT_PAGE);
            toLeftPortal();
        }
        if(v.isPressEvent(KEY_RIGHT_PAGE))
        {
            v.valide(KEY_RIGHT_PAGE);
            toRightPortal();
        }
    }
}

void GlobalMapRender::onChangeZoom()
{
    std::shared_ptr<Map> m(m_map.lock());
    if(!m)
        return;

    float newTile(m_tile < 1.5f ? 4 : m_tile < 2.5f ? 1 : m_tile < 3.5f ? 2 : 3);

    m_pos *= newTile/m_tile;
    m_tile = newTile;
    Event<EventInstantCenterOfViewChanged>::send(EventInstantCenterOfViewChanged(m_pos));
    generateRender(m);
    generateIcons();
}

void GlobalMapRender::update(const sf::Time & elapsedTime)
{
    m_time += elapsedTime.asSeconds();

    if(norm(m_moveDir)>0.2)
    {
        m_pos += m_moveDir*elapsedTime.asSeconds()*m_tile*40.0f;
        m_pos.x = std::max(m_bounds.left, std::min(m_pos.x, m_bounds.left+m_bounds.width));
        m_pos.y = std::max(m_bounds.top, std::min(m_pos.y, m_bounds.top+m_bounds.height));
        Event<EventCenterOfViewChanged>::send(EventCenterOfViewChanged(m_pos));
    }
}

void GlobalMapRender::generateRender(std::shared_ptr<Map> map)
{
    m_mapRender.clear();

    float top(0);
    float left(0);
    float right(0);
    float down(0);
    for(const std::shared_ptr<Room> & r : *map)
    {
        if(!r)
            continue;
        if(!r->isDiscovered())
            continue;
        unsigned int index(m_mapRender.getVertexCount());
        m_mapRender.resize(index+8+r->doors().size()*4);
        sf::FloatRect rect(r->getPos().x*m_tile-m_tile/2, r->getPos().y*m_tile-m_tile/2, r->getSize().x*m_tile, r->getSize().y*m_tile);
        drawFlatQuad(&m_mapRender[index], rect, wallsColor);
        drawFlatQuad(&m_mapRender[index+4], sf::FloatRect(rect.left+m_tile, rect.top+m_tile, rect.width-2*m_tile, rect.height-2*m_tile), groundColor);
        for(unsigned int i(0) ; i < r->doors().size() ; i++)
        {
            const Door & d(r->doors()[i]);
            sf::Vector2i pos(d.pos.toGlobalBlockPos());
            drawFlatQuad(&m_mapRender[index+8+4*i], sf::FloatRect(pos.x*m_tile-m_tile/2, pos.y*m_tile-m_tile/2, m_tile, m_tile), doorColor);
        }

        top = std::min(top, float(r->getPos().y));
        left = std::min(left, float(r->getPos().x));
        right = std::max(right, float(r->getPos().x+(int)r->getSize().x));
        down = std::max(down, float(r->getPos().y+(int)r->getSize().y));
    }
    m_bounds = sf::FloatRect(left*m_tile, top*m_tile, (right-left)*m_tile, (down-top)*m_tile);
}

void GlobalMapRender::createIcons(std::shared_ptr<Map> map)
{
    m_iconsDatas.clear();
    m_portals.clear();

    const unsigned int portalID(8);
    const unsigned int chestID(6);
    const unsigned int saveID(9);

    for(const std::shared_ptr<Room> & r : *map)
    {
        if(!r)
            continue;
        if(!r->isDiscovered())
            continue;

        for(const Door & d : r->doors())
        {
            std::shared_ptr<Room> dest(d.dest.getRoom().lock());
            if(dest)
                if(dest->isDiscovered())
                    continue;

            Orientation o(d.getOrientation());
            float rot(3.14159f/2* (o == DOWN ? 0 : o == LEFT ? 1 : o == UP ? 2 : 3));
            m_iconsDatas.push_back(IconData(sf::Vector2f(d.pos.toGlobalBlockPos()), door, rot));
        }

        for(unsigned int i(0) ; i < r->getSize().x ; i++)
            for(unsigned int j(0) ; j < r->getSize().y ; j++)
            {
                unsigned int id((*r)(sf::Vector2u(i, j)).wallID);
                sf::FloatRect rect;
                if(id == portalID)
                {
                    rect = portal;
                    m_portals.push_back(Location(sf::Vector2u(i, j), r));
                }
                else if(id == chestID)
                    rect = chest;
                else if(id == saveID)
                    rect = save;
                else continue;

                sf::Vector2f pos(int(i)+r->getPos().x, int(j) + r->getPos().y);
                m_iconsDatas.push_back(IconData(pos, rect));
            }

        if(r->type() == BOSS1_ROOM || r->type() == BOSS2_ROOM || r->type() == BOSS3_ROOM || r->type() == BOSS4_ROOM)
        {
            sf::Vector2f center(sf::Vector2f(r->getPos())+sf::Vector2f(r->getSize())/2.0f);
            m_iconsDatas.push_back(IconData(center, boss));
        }
    }

    auto p(EventSimpleGetter<std::shared_ptr<Player>>::get());
    if(p)
    {
        sf::Vector2f pos(p->getPos().toGlobalPos());
        m_iconsDatas.push_back(IconData(pos, player));
    }
}

void GlobalMapRender::generateIcons()
{
    m_iconRender.clear();
    m_iconRender.resize(m_iconsDatas.size()*4);

    for(unsigned int i(0) ; i < m_iconsDatas.size() ; i++)
    {
        const IconData & d(m_iconsDatas[i]);

        drawOrientedQuad(&m_iconRender[i*4], sf::FloatRect(d.pos.x*m_tile-d.tex.width/2, d.pos.y*m_tile-d.tex.height/2, d.tex.width, d.tex.height), d.tex, false, false, ROT_0, d.rot, d.pos*m_tile);
    }
}

void GlobalMapRender::movePortailIDToCurrent()
{
    auto p(EventSimpleGetter<std::shared_ptr<Player>>::get());
    if(!p)
        return;

    unsigned int lowerID(0);
    for(unsigned int i(1) ; i < m_portals.size() ; i++)
        if(norm(m_portals[i].toGlobalPos() - p->getPos().toGlobalPos()) < norm(m_portals[lowerID].toGlobalPos() - p->getPos().toGlobalPos()))
            lowerID = i;

    m_portalID = lowerID;
}

void GlobalMapRender::moveCameraToCurrentPortal(bool instant)
{
    if(m_portals.empty())
        return;
    sf::Vector2f pos(m_portals[m_portalID].toGlobalPos()*m_tile);
    if(instant)
        Event<EventInstantCenterOfViewChanged>::send(EventInstantCenterOfViewChanged(pos));
    else Event<EventCenterOfViewChanged>::send(EventCenterOfViewChanged(pos));
    m_pos = pos;
}


void GlobalMapRender::toLeftPortal()
{
    if(m_portals.empty())
        return;

    if(m_portalID > 0)
        m_portalID --;
    else m_portalID = m_portals.size()-1;
    moveCameraToCurrentPortal();
}

void GlobalMapRender::toRightPortal()
{
    if(m_portals.empty())
        return;

    if(m_portalID < m_portals.size()-1)
        m_portalID++;
    else m_portalID = 0;
    moveCameraToCurrentPortal();
}

Location GlobalMapRender::getCurrentPortal() const
{
    assert(m_canTp);
    assert(!m_portals.empty());

    return m_portals[m_portalID];
}

