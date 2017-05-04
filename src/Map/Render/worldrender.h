#ifndef WORLDRENDER_H
#define WORLDRENDER_H

#include "Utilities/ressource.h"
#include "Events/eventreceiver.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/View.hpp>
#include <vector>

class EventPrePlayerChangeRoom;
class EventSizeViewChanged;
class Map;
class RoomRender;
class Block;

class WorldRender : public sf::Drawable , public EventReceiver
{
public:
    WorldRender(std::weak_ptr<Map> world, unsigned int centerRoom, const sf::Vector2u & screenSize);
    virtual ~WorldRender() = default;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;
    void changeCenterRoom(unsigned int id);
    void changeScreenSize(const sf::Vector2u & size); //en nombre de blocks

    void enable();
    void disable();

private:
    void regenBorder();
    void redrawRooms();
    void onPlayerChangeRoom(EventPrePlayerChangeRoom e);
    void onScreenChangeSize(EventSizeViewChanged e);

    std::weak_ptr<Map> m_map;
    std::vector<std::shared_ptr<RoomRender>> m_renders;
    std::vector<std::shared_ptr<RoomRender>> m_topRenders;
    sf::VertexArray m_border;
    Texture m_borderTexture;
    sf::Vector2u m_screenSize;
    unsigned int m_centerRoom;

    struct OutBlockInfo
    {
        OutBlockInfo(const sf::Vector2i _pos)
            : cornerDL(false), cornerDR(false), cornerUL(false), cornerUR(false)
            , borderD(false), borderL(false), borderR(false), borderU(false)
            , isOn(false)
            , pos(_pos)
        {}
        Block toBlock() const;

        bool cornerDL, cornerDR, cornerUL, cornerUR;
        bool borderD, borderL, borderR, borderU;
        bool isOn;
        sf::Vector2i pos;
    };

    bool m_enabled;
};

#endif // WORLDRENDER_H
