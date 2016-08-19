#ifndef GLOBALMAPRENDER_H
#define GLOBALMAPRENDER_H

#include <memory>
#include <vector>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "Map/location.h"
#include "Utilities/ressource.h"
#include "Systemes/controlable.h"
#include "Systemes/updatable.h"

class Map;

class GlobalMapRender : public sf::Drawable, public Controlable, public Updatable
{
public:
    GlobalMapRender(std::weak_ptr<Map> map, bool canTp);

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;
    virtual void control(CommandsValue & v);
    virtual void update(const sf::Time & elapsedTime);
    Location getCurrentPortal() const;

private:
    struct IconData
    {
        IconData(const sf::Vector2f & _pos, const sf::FloatRect _tex, float _rot = 3.14159f)
            : rot(_rot), pos(_pos), tex(_tex) {}
        float rot;
        sf::Vector2f pos;
        sf::FloatRect tex;
    };

    void onChangeZoom();
    void generateRender(std::shared_ptr<Map> map);
    void createIcons(std::shared_ptr<Map> map);
    void generateIcons();
    void movePortailIDToCurrent();
    void moveCameraToCurrentPortal(bool instant = false);
    void drawPortalCursor(sf::RenderTarget & target) const;
    void toLeftPortal();
    void toRightPortal();

    sf::VertexArray m_mapRender;
    sf::VertexArray m_iconRender;

    Texture m_iconTexture;

    bool m_canTp;
    std::vector<Location> m_portals;
    unsigned int m_portalID;
    sf::Vector2f m_moveDir;
    sf::Vector2f m_pos;
    sf::FloatRect m_bounds;

    float m_tile;
    std::weak_ptr<Map> m_map;
    std::vector<IconData> m_iconsDatas;
    float m_time;
};

#endif // GLOBALMAPRENDER_H
