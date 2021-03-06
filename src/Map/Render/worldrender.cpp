#include "worldrender.h"
#include "Systemes/drawablelist.h"
#include "Utilities/quadrender.h"
#include "Map/blocktype.h"
#include "Entities/entitylist.h"
#include "Events/eventgetter.h"
#include "Events/Datas/Entity/eventpreplayerchangeroom.h"
#include "Events/Datas/Camera/eventsizeviewchanged.h"
#include "Entities/Types/entity.h"
#include "Map/map.h"
#include "roomrender.h"
#include "lightsrender.h"
#include "Map/room.h"
#include <SFML/Graphics/Rect.hpp>
#include <cassert>

WorldRender::WorldRender(std::weak_ptr<Map> world, unsigned int centerRoom, const sf::Vector2u & screenSize)
    : m_map(world)
    , m_border(sf::Quads)
    , m_borderTexture("res/img/border.png")
    , m_screenSize(screenSize / BlockType::tileSize)
    , m_centerRoom(centerRoom)
    , m_enabled(false)
    , m_lightRender(std::make_shared<LightsRender>())
{
    Material m(1, 1, 1, 20);
    m.primaryTexture = m_borderTexture;
    m_shader.setMaterial(m);

    connect<EventPrePlayerChangeRoom>(std::bind(&WorldRender::onPlayerChangeRoom, this, _1));
    connect<EventSizeViewChanged>(std::bind(&WorldRender::onScreenChangeSize, this, _1));
    redrawRooms();
}

void WorldRender::enable()
{
    m_enabled = true;
    for(const auto & r : m_renders)
    {
        DrawableList::add(r, DrawableList::DrawHeight::MAP_BACK);
        Updatable::add(r);
    }
    for(const auto & r : m_topRenders)
    {
        DrawableList::add(r, DrawableList::DrawHeight::MAP_TOP);
        Updatable::add(r);
    }
    Updatable::add(m_lightRender);
}

void WorldRender::disable()
{
    m_enabled = false;
    for(const auto & r : m_renders)
    {
        DrawableList::del(r);
        Updatable::del(r);
    }
    for(const auto & r : m_topRenders)
    {
        DrawableList::del(r);
        Updatable::del(r);
    }
    Updatable::del(m_lightRender);
}

void WorldRender::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::RenderStates state(m_borderTexture());
    state.shader = &m_shader.get();
    target.draw(m_border, state);
}

void WorldRender::regenBorder()
{
    m_border.clear();

    std::vector<OutBlockInfo> blocks;

    for(const auto & roomRender : m_renders)
    {
        std::shared_ptr<Room> rLock(roomRender->getRoom().lock());
        if(!rLock)
            continue;

        for(int i(-1) ; i < int(rLock->getSize().x+1) ; i++)
            for(int j(-1) ; j < int(rLock->getSize().y+1) ; j++)
            {
                sf::Vector2i pos(i, j);
                sf::Vector2i globalPos(pos+rLock->getPos());
                auto it(std::find_if(blocks.begin(), blocks.end(), [globalPos](const auto & b){return b.pos == globalPos;}));
                if(it == blocks.end())
                {
                    blocks.emplace_back(globalPos);
                    it = blocks.end()-1;
                }
                OutBlockInfo & b = *it;
                if(b.isOn)
                    continue;

                if(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(rLock->getSize())).contains(pos))
                    b.isOn = true;
                else
                {
                    if(i == -1 && j == -1)
                        b.cornerDR = true;
                    else if(i == -1 && j == int(rLock->getSize().y))
                        b.cornerUR = true;
                    else if(i == int(rLock->getSize().x) && j == -1)
                        b.cornerDL = true;
                    else if(i == int(rLock->getSize().x) && j == int(rLock->getSize().y))
                        b.cornerUL = true;
                    else if(i == -1)
                    {
                        b.borderL = true;
                        b.cornerDL = true;
                        b.cornerUL = true;
                    }
                    else if(j == -1)
                    {
                        b.borderD = true;
                        b.cornerDL = true;
                        b.cornerDR = true;
                    }
                    else if(i == int(rLock->getSize().x))
                    {
                        b.borderR = true;
                        b.cornerDR = true;
                        b.cornerUR = true;
                    }
                    else if(j == int(rLock->getSize().y))
                    {
                        b.borderU = true;
                        b.cornerUL = true;
                        b.cornerUR = true;
                    }
                    else assert(false); //si ca assert, j'ai fait une couille
                }
            }
    }

    auto it(std::remove_if(blocks.begin(), blocks.end(), [](const auto & b){return b.isOn;}));
    blocks.erase(it, blocks.end());

    m_border.resize(blocks.size()*4);

    for(unsigned int i(0) ; i < blocks.size() ; i++)
    {
        Block b(blocks[i].toBlock());
        sf::Vector2i pos(blocks[i].pos);
        sf::Vertex* quads(&m_border[i*4]);
        sf::Vector2f texPos(b.groundID%BlockType::nbTile, b.groundID/BlockType::nbTile);

        drawQuad(quads, sf::FloatRect((sf::Vector2f(pos)-sf::Vector2f(0.5f, 0.5f))*float(BlockType::tileSize), sf::Vector2f(BlockType::tileSize, BlockType::tileSize))
                 , sf::FloatRect(texPos*float(BlockType::tileSize), sf::Vector2f(BlockType::tileSize, BlockType::tileSize))
                 , getXFlip(b.groundOrientation), getYFlip(b.groundOrientation), getRotation(b.groundOrientation));
    }
}

void WorldRender::redrawRooms()
{
    m_renders.clear();
    m_topRenders.clear();

    std::shared_ptr<Map> m(m_map.lock());
    if(!m)
        return;

    auto centerRoom(m->room(m_centerRoom));
    if(!centerRoom)
        return;
    sf::IntRect rect(centerRoom->getPos()-sf::Vector2i(m_screenSize/2u), sf::Vector2i(centerRoom->getSize()+m_screenSize));

    for(auto & room : *m)
    {
        if(!room)
            continue;
        sf::IntRect rectRoom(room->getPos(), sf::Vector2i(room->getSize()));
        if(rect.intersects(rectRoom))
        {
            bool actual(room == centerRoom);
            m_renders.push_back(std::make_shared<RoomRender>(room, actual, true, true, false));
            m_topRenders.push_back(std::make_shared<RoomRender>(room, actual, false, false, true));
            if(m_enabled)
            {
                DrawableList::add(m_renders.back(), DrawableList::DrawHeight::MAP_BACK);
                Updatable::add(m_renders.back());
                DrawableList::add(m_topRenders.back(), DrawableList::DrawHeight::MAP_TOP);
                Updatable::add(m_topRenders.back());
            }
        }
    }

    std::vector<std::weak_ptr<Room>> rooms;
    for(auto & r : m_renders)
        rooms.push_back(r->getRoom());
    m_lightRender->setRooms(rooms);

    regenBorder();
}

void WorldRender::changeCenterRoom(unsigned int id)
{
    m_centerRoom = id;
    redrawRooms();
}

void WorldRender::changeScreenSize(const sf::Vector2u & size)
{
    m_screenSize = size / BlockType::tileSize;
    redrawRooms();
}

void WorldRender::onScreenChangeSize(EventSizeViewChanged e)
{
    changeScreenSize(sf::Vector2u(e.size));
}

void WorldRender::onPlayerChangeRoom(EventPrePlayerChangeRoom e)
{
    std::shared_ptr<Entity>eLock(EventGetter<std::shared_ptr<Entity>,unsigned int>::get(e.entityID));
    //std::shared_ptr<Entity>eLock(EntityList::list().entity(e.entityID));
    if(!eLock)
        return;
    std::shared_ptr<Room> r(eLock->getPos().getRoom().lock());
    if(!r)
        return;
    r->uncover();
    changeCenterRoom(r->getID());
}

Block WorldRender::OutBlockInfo::toBlock() const
{
    const unsigned int oneCornerID(0);
    const unsigned int twoCornersSideID(1);
    const unsigned int twoCornersOppositeID(2);
    const unsigned int treeCornersID(3);
    const unsigned int fourCornersID(4);
    const unsigned int oneSideID(5);
    const unsigned int twoOppositeSideID(6);
    const unsigned int oneSideAndOneCornerID(7);
    const unsigned int oneSideAndTwoCornersID(8);
    const unsigned int twoSidesInCornerID(9);
    const unsigned int treeSidesID(10);
    const unsigned int fourSidesID(11);
    const unsigned int twoSidesInCornerAndOneCornerID(12);

    assert(!isOn);

    //4 cotés
    if(borderD && borderL && borderR && borderU)
        return Block(fourSidesID, 0);
    //3 cotés
    if(borderD && borderL && borderR)
        return Block(treeSidesID, createOrientation(Rotation::ROT_0, false, false));
    if(borderD && borderR && borderU)
        return Block(treeSidesID, createOrientation(Rotation::ROT_270, false, false));
    if(borderL && borderR && borderU)
        return Block(treeSidesID, createOrientation(Rotation::ROT_180, false, false));
    if(borderD && borderL && borderU)
        return Block(treeSidesID, createOrientation(Rotation::ROT_90, false, false));
    //2 cotés face à face
    if(borderD && borderU)
        return Block(twoOppositeSideID, createOrientation(Rotation::ROT_0, false, false));
    if(borderL && borderR)
        return Block(twoOppositeSideID, createOrientation(Rotation::ROT_270, false, false));
    //2 cotés en coin sans coin a l'opposé
    if(borderL && borderD && !cornerUR)
        return Block(twoSidesInCornerID, createOrientation(Rotation::ROT_90, false, false));
    if(borderD && borderR && !cornerUL)
        return Block(twoSidesInCornerID, createOrientation(Rotation::ROT_0, false, false));
    if(borderR && borderU && !cornerDL)
        return Block(twoSidesInCornerID, createOrientation(Rotation::ROT_270, false, false));
    if(borderU && borderL && !cornerDR)
        return Block(twoSidesInCornerID, createOrientation(Rotation::ROT_180, false, false));
    //2 cotés en coin avec 1 coin
    if(borderL && borderD)
        return Block(twoSidesInCornerAndOneCornerID, createOrientation(Rotation::ROT_0, false, false));
    if(borderD && borderR)
        return Block(twoSidesInCornerAndOneCornerID, createOrientation(Rotation::ROT_270, false, false));
    if(borderR && borderU)
        return Block(twoSidesInCornerAndOneCornerID, createOrientation(Rotation::ROT_180, false, false));
    if(borderU && borderL)
        return Block(twoSidesInCornerAndOneCornerID, createOrientation(Rotation::ROT_90, false, false));
    //1 coté sans coins
    if(borderD && !cornerUL && !cornerUR)
        return Block(oneSideID, createOrientation(Rotation::ROT_0, false, false));
    if(borderR && !cornerDL && !cornerUL)
        return Block(oneSideID, createOrientation(Rotation::ROT_270, false, false));
    if(borderU && !cornerDL && !cornerDR)
        return Block(oneSideID, createOrientation(Rotation::ROT_180, false, false));
    if(borderL && !cornerDR && !cornerUR)
        return Block(oneSideID, createOrientation(Rotation::ROT_90, false, false));
    //1 coté et 1 coin
    if(borderD && !cornerUR)
        return Block(oneSideAndOneCornerID, createOrientation(Rotation::ROT_0, false, false));
    if(borderD && !cornerUL)
        return Block(oneSideAndOneCornerID, createOrientation(Rotation::ROT_0, true, false));
    if(borderR && !cornerUL)
        return Block(oneSideAndOneCornerID, createOrientation(Rotation::ROT_270, false, false));
    if(borderR && !cornerDL)
        return Block(oneSideAndOneCornerID, createOrientation(Rotation::ROT_270, false, true));
    if(borderU && !cornerDL)
        return Block(oneSideAndOneCornerID, createOrientation(Rotation::ROT_180, false, false));
    if(borderU && !cornerDR)
        return Block(oneSideAndOneCornerID, createOrientation(Rotation::ROT_180, true, false));
    if(borderL && !cornerUR)
        return Block(oneSideAndOneCornerID, createOrientation(Rotation::ROT_90, false, false));
    if(borderL && !cornerDR)
        return Block(oneSideAndOneCornerID, createOrientation(Rotation::ROT_90, false, true));
    //1 coté et 2 coins
    if(borderD)
        return Block(oneSideAndTwoCornersID, createOrientation(Rotation::ROT_0, false, false));
    if(borderR)
        return Block(oneSideAndTwoCornersID, createOrientation(Rotation::ROT_270, false, false));
    if(borderU)
        return Block(oneSideAndTwoCornersID, createOrientation(Rotation::ROT_180, false, false));
    if(borderL)
        return Block(oneSideAndTwoCornersID, createOrientation(Rotation::ROT_90, false, false));
    //4 coins
    if(cornerDL && cornerDR && cornerUL && cornerUR)
        return Block(fourCornersID, createOrientation(Rotation::ROT_0, false, false));
    //3 coins
    if(cornerDL && cornerDR && cornerUL)
        return Block(treeCornersID, createOrientation(Rotation::ROT_0, false, false));
    if(cornerDL && cornerDR && cornerUR)
        return Block(treeCornersID, createOrientation(Rotation::ROT_270, false, false));
    if(cornerDR && cornerUL && cornerUR)
        return Block(treeCornersID, createOrientation(Rotation::ROT_180, false, false));
    if(cornerDL && cornerUL && cornerUR)
        return Block(treeCornersID, createOrientation(Rotation::ROT_90, false, false));
    //2 coins cote a cote
    if(cornerDL && cornerDR)
        return Block(twoCornersSideID, createOrientation(Rotation::ROT_0, false, false));
    if(cornerDR && cornerUR)
        return Block(twoCornersSideID, createOrientation(Rotation::ROT_270, false, false));
    if(cornerUL && cornerUR)
        return Block(twoCornersSideID, createOrientation(Rotation::ROT_180, false, false));
    if(cornerDL && cornerUL)
        return Block(twoCornersSideID, createOrientation(Rotation::ROT_90, false, false));
    //2 coins face a face
    if(cornerDL && cornerUR)
        return Block(twoCornersOppositeID, createOrientation(Rotation::ROT_0, false, false));
    if(cornerDR && cornerUL)
        return Block(twoCornersOppositeID, createOrientation(Rotation::ROT_270, false, false));
    //1 coin
    if(cornerDL)
        return Block(oneCornerID, createOrientation(Rotation::ROT_0, false, false));
    if(cornerUL)
        return Block(oneCornerID, createOrientation(Rotation::ROT_270, false, false));
    if(cornerUR)
        return Block(oneCornerID, createOrientation(Rotation::ROT_180, false, false));
    if(cornerDR)
        return Block(oneCornerID, createOrientation(Rotation::ROT_90, false, false));

    assert(false); //retour normalement impossible
    return Block();
}
