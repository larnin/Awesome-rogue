#include "cacaura.h"
#include "Events/eventgetter.h"
#include "Map/room.h"
#include "Utilities/vect2convert.h"

CacAura::CacAura(const Location & pos, Team team, std::weak_ptr<Entity> sender, float radius)
    : Projectile(pos, team, sender)
    , m_radius(radius)
{

}

void CacAura::update(const sf::Time &)
{
    if(m_killed)
        return;

    std::shared_ptr<Entity> e(m_sender.lock());
    if(!e)
        m_killed = true;

    m_pos = e->getPos();

    std::shared_ptr<Room> r(m_pos.getRoom().lock());
    if(!r)
        return;

    std::vector<std::shared_ptr<Entity>> entities(EventGetter<std::vector<std::shared_ptr<Entity>>,unsigned int>::get(r->getID()));
    float dmgValue(m_radius*5);
    for(std::shared_ptr<Entity> & entity : entities)
    {
        if(!entity)
            continue;
        if(entity->getTeam() == m_team)
            continue;
        sf::Vector2f vect(entity->getPos().getPos() - m_pos.getPos());
        if(norm(vect) < m_radius)
            entity->damage(dmgValue, m_sender, normalise(vect));
    }
}

void CacAura::draw(sf::RenderTarget &, sf::RenderStates) const
{

}
